"use strict";

/*
 * ============================================================
 * Fusion 360 WebGL Viewer
 * ============================================================
 */

/*
 * ============================================================
 * Constants
 * ============================================================
 */

const MOCK = false;
const frontVideoPath = "";
const backVideoPath  = "";

const SHOW_CALIBRATION_PANEL = false;

/* ============================================================
 * Early stub: QML may call loadVideos() before this script
 * has finished. Queue and replay instead of throwing.
 * ============================================================ */
window.fusionViewer = {
    _isReady: false,
    _pendingVideos: null,
    _pendingPlay: false,

    loadVideos(front, back) {
        if (this._isReady) setVideoSources(front, back);
        else { console.log("[viewer] loadVideos queued"); this._pendingVideos = [front, back]; }
    },
    play()  { this._isReady ? (frontVideo.paused && togglePlayback()) : (this._pendingPlay = true); },
    pause() { this._isReady && !frontVideo.paused && togglePlayback(); }
};

/*
 * ============================================================
 * DOM
 * ============================================================
 */

const canvas          = document.getElementById("glCanvas");
const frontVideo      = document.getElementById("frontVideo");
const backVideo       = document.getElementById("backVideo");
const loading         = document.getElementById("loading");
const playButton      = document.getElementById("playButton");
const progress        = document.getElementById("progress");
const timeLabel       = document.getElementById("time");
const fullscreenButton= document.getElementById("fullscreenButton");

/*
 * ============================================================
 * QML bridge
 * ============================================================
 */

let qmlBridge = null;

if (MOCK) {
    qmlBridge = {
        ready(data)            { console.log("[QML MOCK] Viewer ready:", data); },
        playingChanged(playing){ console.log("[QML MOCK] Playing:", playing); },
        cameraChanged(data)    { console.log("[QML MOCK] Camera:", data); }
    };
} else {
    if (typeof qt === "undefined" || !qt.webChannelTransport) {
        console.warn("QWebChannel transport not available – running standalone");
    } else {
        new QWebChannel(qt.webChannelTransport, function (channel) {
            qmlBridge = channel.objects.fusionViewerBridge;
            console.log("QWebChannel initialized, bridge =", qmlBridge);
            notifyQML("ready", { initialized: true });
        });
    }
}

/*
 * ============================================================
 * WebGL
 * ============================================================
 */

const gl = canvas.getContext("webgl", { alpha: false, antialias: true });

if (!gl) {
    showError("WebGL is not available.");
    throw new Error("WebGL unavailable");
}

/*
 * ============================================================
 * Vertex shader
 * ============================================================
 */

const vertexShaderSource = `
attribute vec2 a_position;
varying   vec2 v_uv;

void main() {
    v_uv        = a_position * 0.5 + 0.5;
    gl_Position = vec4(a_position, 0.0, 1.0);
}
`;

/*
 * ============================================================
 * Fragment shader
 * ============================================================
 *
 * Mirror support:
 *
 *   u_frontMirror / u_backMirror accept +1.0 or -1.0.
 *   Negating the local angle phi of the fisheye projection
 *   mirrors the sampled image around its horizontal axis.
 *   This is exactly what is needed when one lens of the
 *   Fusion was stored with the opposite handedness.
 * ============================================================
 */

const fragmentShaderSource = `
precision highp float;

uniform sampler2D u_front;
uniform sampler2D u_back;

uniform vec2  u_resolution;
uniform float u_fov;

uniform vec2  u_frontCenter;
uniform vec2  u_backCenter;
uniform vec2  u_frontRadius;
uniform vec2  u_backRadius;
uniform float u_frontRotation;
uniform float u_backRotation;
uniform float u_frontMirror;
uniform float u_backMirror;

uniform float u_blendWidth;
uniform float u_frontExposure;
uniform float u_backExposure;
uniform float u_ghostSharpness;

uniform float u_yaw;
uniform float u_pitch;
uniform float u_focal;

uniform float u_viewMode;
uniform float u_showEdges;

varying vec2 v_uv;

mat3 rotationX(float a) {
    float c = cos(a); float s = sin(a);
    return mat3(1.0, 0.0, 0.0,
                0.0, c,  -s,
                0.0, s,   c);
}

mat3 rotationY(float a) {
    float c = cos(a); float s = sin(a);
    return mat3( c,   0.0, s,
                 0.0, 1.0, 0.0,
                -s,   0.0, c);
}

vec3 fisheyeUV(vec3 direction, vec3 lensForward,
               vec2 center, vec2 radius,
               float rotation, float mirror, float fov)
{
    float cosTheta = clamp(dot(direction, lensForward), -1.0, 1.0);
    float theta    = acos(cosTheta);
    float maxTheta = radians(fov * 0.5);

    if (theta > maxTheta) return vec3(-1.0, -1.0, -1.0);

    vec3 tangent = direction - lensForward * cosTheta;
    float tl     = length(tangent);
    if (tl < 0.00001) return vec3(center, 0.0);
    tangent /= tl;

    vec3 up = vec3(0.0, 1.0, 0.0);
    if (abs(dot(up, lensForward)) > 0.99) up = vec3(1.0, 0.0, 0.0);

    vec3 right = normalize(cross(up, lensForward));
    up         = normalize(cross(lensForward, right));

    float x   = dot(tangent, right);
    float y   = dot(tangent, up);

    /* Mirror: negating phi flips the image around its
       horizontal axis (mirror of the fisheye circle). */
    float phi = atan(y, x) * mirror + rotation;

    float r   = theta / maxTheta;

    vec2 uv = center + vec2(cos(phi) * radius.x * r,
                            sin(phi) * radius.y * r);

    return vec3(uv, r);
}

float frontBlendWeight(vec3 direction) {
    float longitude = degrees(atan(direction.x, direction.z));
    float la        = abs(longitude);
    float start     = 90.0 - u_blendWidth;
    float end       = 90.0 + u_blendWidth;
    if (la <= start) return 1.0;
    if (la >= end)   return 0.0;
    float t = (la - start) / (end - start);
    return 0.5 * (1.0 + cos(t * 3.14159265359));
}

void main() {
    vec2  p      = v_uv * 2.0 - 1.0;
    float aspect = u_resolution.x / u_resolution.y;
    p.x *= aspect;

    vec3 direction = normalize(vec3(p.x, -p.y, u_focal));
    direction = rotationY(u_yaw) * rotationX(u_pitch) * direction;

    vec3 frontForward = vec3(0.0, 0.0,  1.0);
    vec3 backForward  = vec3(0.0, 0.0, -1.0);

    vec3 fr = fisheyeUV(direction, frontForward,
                        u_frontCenter, u_frontRadius,
                        u_frontRotation, u_frontMirror, u_fov);
    vec3 br = fisheyeUV(direction, backForward,
                        u_backCenter, u_backRadius,
                        u_backRotation, u_backMirror, u_fov);

    vec2  frontUV = fr.xy;  float frontR = fr.z;
    vec2  backUV  = br.xy;  float backR  = br.z;

    frontUV.y = 1.0 - frontUV.y;

    bool frontValid =
        frontR >= 0.0 &&
        frontUV.x >= 0.0 && frontUV.x <= 1.0 &&
        frontUV.y >= 0.0 && frontUV.y <= 1.0;

    bool backValid =
        backR >= 0.0 &&
        backUV.x >= 0.0 && backUV.x <= 1.0 &&
        backUV.y >= 0.0 && backUV.y <= 1.0;

    float frontWeight = frontBlendWeight(direction);
    float backWeight  = 1.0 - frontWeight;

    if (!frontValid) frontWeight = 0.0;
    if (!backValid)  backWeight  = 0.0;

    if (frontValid && backValid &&
        frontWeight > 0.0001 && backWeight > 0.0001)
    {
        float fc = max(0.2, 1.0 - frontR);
        float bc = max(0.2, 1.0 - backR);
        frontWeight *= pow(fc, u_ghostSharpness);
        backWeight  *= pow(bc, u_ghostSharpness);
    }

    vec4 frontColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 backColor  = vec4(0.0, 0.0, 0.0, 1.0);

    if (frontValid) {
        frontColor     = texture2D(u_front, frontUV);
        frontColor.rgb *= u_frontExposure;
    }
    if (backValid) {
        backColor     = texture2D(u_back, backUV);
        backColor.rgb *= u_backExposure;
    }

    float total = frontWeight + backWeight;

    vec4 outColor;

    if (u_viewMode > 2.5 && u_viewMode < 3.5) {
        if (v_uv.x < 0.5) {
            outColor = frontValid ? frontColor : vec4(0.15, 0.0, 0.0, 1.0);
        } else {
            outColor = backValid ? backColor : vec4(0.0, 0.15, 0.0, 1.0);
        }
    }
    else if (u_viewMode > 3.5 && u_viewMode < 4.5) {
        float rW = frontValid ? frontWeight : 0.0;
        float gW = backValid  ? backWeight  : 0.0;
        outColor = vec4(rW, gW, 0.0, 1.0);
    }
    else if (u_viewMode > 0.5 && u_viewMode < 1.5) {
        outColor = frontValid ? frontColor : vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if (u_viewMode > 1.5 && u_viewMode < 2.5) {
        outColor = backValid ? backColor : vec4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        if (total > 0.0001) {
            outColor = (frontColor * frontWeight + backColor * backWeight) / total;
        } else {
            outColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

    if (u_showEdges > 0.5) {
        float edgeWidth = 0.02;
        if (frontR >= 0.0 && frontR > 1.0 - edgeWidth) {
            outColor.rgb = mix(outColor.rgb, vec3(1.0, 0.25, 0.25), 0.9);
        }
        if (backR >= 0.0 && backR > 1.0 - edgeWidth) {
            outColor.rgb = mix(outColor.rgb, vec3(0.25, 1.0, 0.35), 0.9);
        }
    }

    gl_FragColor = outColor;
}
`;

/*
 * ============================================================
 * Shader compilation
 * ============================================================
 */

function compileShader(type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        const error = gl.getShaderInfoLog(shader);
        gl.deleteShader(shader);
        throw new Error(error);
    }
    return shader;
}

const vertexShader   = compileShader(gl.VERTEX_SHADER,   vertexShaderSource);
const fragmentShader = compileShader(gl.FRAGMENT_SHADER, fragmentShaderSource);

const program = gl.createProgram();
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);
gl.linkProgram(program);

if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    throw new Error(gl.getProgramInfoLog(program));
}
gl.useProgram(program);

/*
 * ============================================================
 * Full-screen quad
 * ============================================================
 */

const buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
    -1, -1,  1, -1, -1,  1,
    -1,  1,  1, -1,  1,  1
]), gl.STATIC_DRAW);

const positionLocation = gl.getAttribLocation(program, "a_position");
gl.enableVertexAttribArray(positionLocation);
gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);

/*
 * ============================================================
 * Uniforms
 * ============================================================
 */

const uniforms = {
    resolution:     gl.getUniformLocation(program, "u_resolution"),
    front:          gl.getUniformLocation(program, "u_front"),
    back:           gl.getUniformLocation(program, "u_back"),
    fov:            gl.getUniformLocation(program, "u_fov"),
    frontCenter:    gl.getUniformLocation(program, "u_frontCenter"),
    backCenter:     gl.getUniformLocation(program, "u_backCenter"),
    frontRadius:    gl.getUniformLocation(program, "u_frontRadius"),
    backRadius:     gl.getUniformLocation(program, "u_backRadius"),
    frontRotation:  gl.getUniformLocation(program, "u_frontRotation"),
    backRotation:   gl.getUniformLocation(program, "u_backRotation"),
    frontMirror:    gl.getUniformLocation(program, "u_frontMirror"),
    backMirror:     gl.getUniformLocation(program, "u_backMirror"),
    blendWidth:     gl.getUniformLocation(program, "u_blendWidth"),
    frontExposure:  gl.getUniformLocation(program, "u_frontExposure"),
    backExposure:   gl.getUniformLocation(program, "u_backExposure"),
    ghostSharpness: gl.getUniformLocation(program, "u_ghostSharpness"),
    yaw:            gl.getUniformLocation(program, "u_yaw"),
    pitch:          gl.getUniformLocation(program, "u_pitch"),
    focal:          gl.getUniformLocation(program, "u_focal"),
    viewMode:       gl.getUniformLocation(program, "u_viewMode"),
    showEdges:      gl.getUniformLocation(program, "u_showEdges")
};

/*
 * ============================================================
 * Textures
 * ============================================================
 */

const frontTexture = gl.createTexture();
const backTexture  = gl.createTexture();

function initializeTexture(texture) {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
}
initializeTexture(frontTexture);
initializeTexture(backTexture);

/*
 * ============================================================
 * Camera
 * ============================================================
 */

let yaw   = 0.0;
let pitch = 0.0;
let focal = 1.0;

/*
 * ============================================================
 * Fusion calibration
 * ============================================================
 *
 * mirror: +1 = normal, -1 = mirrored.
 * If one lens appears as a mirror image of the other, flip
 * its mirror flag.
 * ============================================================
 */

/* ============================================================
 * Calibration profiles
 * ============================================================
 *
 * Each profile is a complete stitcher configuration. The
 * "fusion_3k_50" profile is the built-in default.
 *
 * QML can select a profile by name via
 *     window.fusionViewer.setProfile("some_name")
 * or supply a fresh object via
 *     window.fusionViewer.setCalibration({...})
 * ============================================================ */

const DEFAULT_PROFILE_NAME = "fusion_3k_50";

const PROFILES = {
    "fusion_3k_50": {
        fov: 201,
        blendWidth: 5,
        ghostSharpness: 1,
        front: {
            center:   [0.4974, 0.4965],
            radius:   [0.51, 0.51],
            rotation: -Math.PI,
            mirror:   -1
        },
        back: {
            center:   [0.4944, 0.4965],
            radius:   [0.51, 0.51],
            rotation: Math.PI,
            mirror:   1
        }
    }
};

/* Deep-copy a profile so the live calibration object never
 * aliases the stored one. */
function cloneProfile(p) {
    return JSON.parse(JSON.stringify(p));
}

/* Live calibration – always a copy of whatever profile is active. */
const calibration = cloneProfile(PROFILES[DEFAULT_PROFILE_NAME]);

/* Kept in sync with whatever profile is active, so "Reset"
 * restores that profile rather than the boot-time one. */
let initialCalibration = cloneProfile(PROFILES[DEFAULT_PROFILE_NAME]);
let currentProfileName  = DEFAULT_PROFILE_NAME;

/*
 * ============================================================
 * Debug UI state
 * ============================================================
 */

let currentViewMode   = 0.0;
let currentShowEdges  = 1.0;
let autoExposureOn    = true;

/*
 * ============================================================
 * Slider definitions
 * ============================================================
 */

const DEG = 180.0 / Math.PI;

const sliderDefinitions = [
    { group: "Front lens",
        id: "frontCenterX", label: "Center X",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.front.center[0],
        set: v => { calibration.front.center[0] = v; } },

    { group: "Front lens",
        id: "frontCenterY", label: "Center Y",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.front.center[1],
        set: v => { calibration.front.center[1] = v; } },

    { group: "Front lens",
        id: "frontRadiusX", label: "Radius X",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.front.radius[0],
        set: v => { calibration.front.radius[0] = v; } },

    { group: "Front lens",
        id: "frontRadiusY", label: "Radius Y",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.front.radius[1],
        set: v => { calibration.front.radius[1] = v; } },

    { group: "Front lens",
        id: "frontRotation", label: "Rotation (°)",
        min: -180, max: 180, step: 0.25, precision: 2,
        get: () => calibration.front.rotation * DEG,
        set: v => { calibration.front.rotation = v / DEG; } },

    { group: "Back lens",
        id: "backCenterX", label: "Center X",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.back.center[0],
        set: v => { calibration.back.center[0] = v; } },

    { group: "Back lens",
        id: "backCenterY", label: "Center Y",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.back.center[1],
        set: v => { calibration.back.center[1] = v; } },

    { group: "Back lens",
        id: "backRadiusX", label: "Radius X",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.back.radius[0],
        set: v => { calibration.back.radius[0] = v; } },

    { group: "Back lens",
        id: "backRadiusY", label: "Radius Y",
        min: 0, max: 1, step: 0.0005, precision: 4,
        get: () => calibration.back.radius[1],
        set: v => { calibration.back.radius[1] = v; } },

    { group: "Back lens",
        id: "backRotation", label: "Rotation (°)",
        min: -180, max: 180, step: 0.25, precision: 2,
        get: () => calibration.back.rotation * DEG,
        set: v => { calibration.back.rotation = v / DEG; } },

    { group: "Global stitching",
        id: "fov", label: "Lens FOV (°)",
        min: 120, max: 220, step: 0.5, precision: 1,
        get: () => calibration.fov,
        set: v => { calibration.fov = v; } },

    { group: "Global stitching",
        id: "blendWidth", label: "Blend half-width (°)",
        min: 0, max: 30, step: 0.25, precision: 2,
        get: () => calibration.blendWidth,
        set: v => { calibration.blendWidth = v; } },

    { group: "Global stitching",
        id: "ghostSharpness", label: "Ghost reduction",
        min: 0, max: 3, step: 0.05, precision: 2,
        get: () => calibration.ghostSharpness,
        set: v => { calibration.ghostSharpness = v; } }
];

/*
 * ============================================================
 * Slider DOM construction
 * ============================================================
 */

const slidersContainer = document.getElementById("slidersContainer");
const sliderUpdaters    = [];

let lastGroup = null;

sliderDefinitions.forEach(function (def) {
    if (def.group !== lastGroup) {
        const t = document.createElement("div");
        t.className   = "slider-group-title";
        t.textContent = def.group;
        slidersContainer.appendChild(t);
        lastGroup = def.group;
    }

    const row = document.createElement("div");
    row.className = "slider-row";

    const labelRow = document.createElement("div");
    labelRow.className = "slider-row-label";

    const labelSpan = document.createElement("span");
    labelSpan.textContent = def.label;

    const valueSpan = document.createElement("span");
    valueSpan.className = "slider-row-value";

    labelRow.appendChild(labelSpan);
    labelRow.appendChild(valueSpan);

    const input = document.createElement("input");
    input.type = "range";
    input.min  = def.min;
    input.max  = def.max;
    input.step = def.step;

    function sync() {
        const v = def.get();
        input.value          = v;
        valueSpan.textContent = v.toFixed(def.precision);
    }

    input.addEventListener("input", function () {
        def.set(parseFloat(input.value));
        valueSpan.textContent = parseFloat(input.value).toFixed(def.precision);
    });

    row.appendChild(labelRow);
    row.appendChild(input);
    slidersContainer.appendChild(row);

    sliderUpdaters.push({ sync });
});

function syncAllSliders() {
    sliderUpdaters.forEach(u => u.sync());
}

syncAllSliders();

/*
 * ============================================================
 * Debug panel wiring
 * ============================================================
 */

const debugPanel = document.getElementById("debugPanel");
const debugToggle = document.getElementById("debugToggle");

debugToggle.addEventListener("click", function (e) {
    e.stopPropagation();
    debugPanel.classList.toggle("collapsed");
    debugToggle.textContent =
        debugPanel.classList.contains("collapsed") ? "+" : "−";
});

document.getElementById("viewMode").addEventListener("change", function (e) {
    currentViewMode = parseFloat(e.target.value);
});

document.getElementById("showEdges").addEventListener("change", function (e) {
    currentShowEdges = (ALLOW_EDGE_OVERLAY && e.target.checked) ? 1.0 : 0.0;
});

document.getElementById("autoExposure").addEventListener("change", function (e) {
    autoExposureOn = e.target.checked;
});

/* --- Mirror toggles --- */

const frontMirrorCheckbox = document.getElementById("frontMirror");
const backMirrorCheckbox  = document.getElementById("backMirror");

frontMirrorCheckbox.addEventListener("change", function (e) {
    calibration.front.mirror = e.target.checked ? -1.0 : 1.0;
});

backMirrorCheckbox.addEventListener("change", function (e) {
    calibration.back.mirror = e.target.checked ? -1.0 : 1.0;
});

function syncMirrorCheckboxes() {
    frontMirrorCheckbox.checked = calibration.front.mirror < 0.0;
    backMirrorCheckbox.checked  = calibration.back.mirror  < 0.0;
}

syncMirrorCheckboxes();

if (!SHOW_CALIBRATION_PANEL) {
    debugPanel.style.display = "none";

    const edgeCheckbox = document.getElementById("showEdges");
    if (edgeCheckbox) {
        edgeCheckbox.checked = false;
        edgeCheckbox.disabled = true;
    }
    currentShowEdges = 0.0;
}

function applyProfile(profile, name) {
    calibration.fov            = profile.fov;
    calibration.blendWidth     = profile.blendWidth;
    calibration.ghostSharpness = profile.ghostSharpness;

    calibration.front = cloneProfile(profile.front);
    calibration.back  = cloneProfile(profile.back);

    initialCalibration = cloneProfile(profile);
    if (name !== undefined) currentProfileName = name;

    syncAllSliders();
    syncMirrorCheckboxes();
}

/* --- Buttons --- */

document.getElementById("copyCalibration").addEventListener("click", function (e) {
    const json = JSON.stringify(calibration, null, 4);
    console.log("=== Fusion calibration ===\n" + json);

    const button = e.currentTarget;
    const original = button.textContent;

    function flash(msg) {
        button.textContent = msg;
        button.classList.add("copied");
        setTimeout(function () {
            button.textContent = original;
            button.classList.remove("copied");
        }, 1200);
    }

    if (navigator.clipboard && navigator.clipboard.writeText) {
        navigator.clipboard.writeText(json)
            .then(() => flash("Copied ✓"))
            .catch(() => flash("See console"));
    } else {
        flash("See console");
    }
});

document.getElementById("resetCalibration").addEventListener("click", function () {
    applyProfile(initialCalibration, currentProfileName);
});

document.getElementById("resetView").addEventListener("click", function () {
    yaw   = 0.0;
    pitch = 0.0;
    focal = 1.0;
});

/*
 * ============================================================
 * Exposure compensation
 * ============================================================
 */

const exposureSampleSize = 32;

function createExposureSampler() {
    const c = document.createElement("canvas");
    c.width = exposureSampleSize;
    c.height = exposureSampleSize;
    return {
        canvas:  c,
        context: c.getContext("2d", { willReadFrequently: true })
    };
}

const frontExposureSampler = createExposureSampler();
const backExposureSampler  = createExposureSampler();

let targetFrontExposure  = 1.0;
let targetBackExposure   = 1.0;
let currentFrontExposure = 1.0;
let currentBackExposure  = 1.0;

function measureLuminance(video, sampler) {
    if (video.readyState < video.HAVE_CURRENT_DATA) return null;

    const size = exposureSampleSize;
    try {
        sampler.context.drawImage(video, 0, 0, size, size);
    } catch (error) {
        return null;
    }

    const pixels = sampler.context.getImageData(0, 0, size, size).data;
    let sum = 0.0;
    let count = 0;

    for (let i = 0; i < pixels.length; i += 4) {
        const r = pixels[i];
        const g = pixels[i + 1];
        const b = pixels[i + 2];

        if (r < 8 && g < 8 && b < 8) continue;

        sum += 0.2126 * r + 0.7152 * g + 0.0722 * b;
        count++;
    }

    if (count === 0) return null;
    return sum / count;
}

let lastExposureSampleTime = 0;
const exposureSampleInterval = 400;

function updateExposureTargets(timestamp) {
    if (!autoExposureOn) return;
    if (timestamp - lastExposureSampleTime < exposureSampleInterval) return;
    lastExposureSampleTime = timestamp;

    const frontLum = measureLuminance(frontVideo, frontExposureSampler);
    const backLum  = measureLuminance(backVideo,  backExposureSampler);

    if (frontLum === null || backLum === null) return;
    if (frontLum < 1.0 || backLum < 1.0) return;

    const average = (frontLum + backLum) * 0.5;
    targetFrontExposure = Math.min(2.5, Math.max(0.4, average / frontLum));
    targetBackExposure  = Math.min(2.5, Math.max(0.4, average / backLum));
}

function smoothExposure() {
    const alpha = 0.08;
    currentFrontExposure += (targetFrontExposure - currentFrontExposure) * alpha;
    currentBackExposure  += (targetBackExposure  - currentBackExposure)  * alpha;
}

/*
 * ============================================================
 * Resize
 * ============================================================
 */

function resize() {
    const dpr = window.devicePixelRatio || 1;
    const w = Math.floor(canvas.clientWidth  * dpr);
    const h = Math.floor(canvas.clientHeight * dpr);

    if (canvas.width !== w || canvas.height !== h) {
        canvas.width  = w;
        canvas.height = h;
        gl.viewport(0, 0, w, h);
    }
}

window.addEventListener("resize", resize);

/*
 * ============================================================
 * Mouse camera controls
 * ============================================================
 */

let dragging = false;
let lastX = 0;
let lastY = 0;

canvas.addEventListener("mousedown", function (event) {
    dragging = true;
    canvas.classList.add("dragging");
    lastX = event.clientX;
    lastY = event.clientY;
});

window.addEventListener("mouseup", function () {
    dragging = false;
    canvas.classList.remove("dragging");
});

window.addEventListener("mousemove", function (event) {
    if (!dragging) return;

    const dx = event.clientX - lastX;
    const dy = event.clientY - lastY;
    lastX = event.clientX;
    lastY = event.clientY;

    yaw   -= dx * 0.005;
    pitch -= dy * 0.005;

    const limit = Math.PI / 2 - 0.01;
    pitch = Math.max(-limit, Math.min(limit, pitch));

    notifyQML("cameraChanged", { yaw: yaw, pitch: pitch });
});

canvas.addEventListener("wheel", function (event) {
    event.preventDefault();
    focal += event.deltaY * -0.001;
    focal = Math.max(0.5, Math.min(2.5, focal));
}, { passive: false });

/*
 * ============================================================
 * Touch
 * ============================================================
 */

let touchX = 0;
let touchY = 0;

canvas.addEventListener("touchstart", function (event) {
    if (event.touches.length !== 1) return;
    touchX = event.touches[0].clientX;
    touchY = event.touches[0].clientY;
}, { passive: true });

canvas.addEventListener("touchmove", function (event) {
    if (event.touches.length !== 1) return;

    const x = event.touches[0].clientX;
    const y = event.touches[0].clientY;

    yaw   -= (x - touchX) * 0.005;
    pitch -= (y - touchY) * 0.005;

    touchX = x;
    touchY = y;

    event.preventDefault();
}, { passive: false });

/*
 * ============================================================
 * Video loading
 * ============================================================
 */

function setVideoSources(frontUrl, backUrl) {
    console.log("Loading front video:", frontUrl);
    console.log("Loading back video:",  backUrl);

    loading.textContent   = "Loading videos...";
    loading.style.display = "block";

    frontVideo.src = frontUrl;
    backVideo.src  = backUrl;
    frontVideo.load();
    backVideo.load();

    Promise.all([waitForMetadata(frontVideo), waitForMetadata(backVideo)])
        .then(function () {
            loading.style.display = "none";

            const duration = Math.min(frontVideo.duration, backVideo.duration);

            notifyQML("ready", {
                frontWidth:  frontVideo.videoWidth,
                frontHeight: frontVideo.videoHeight,
                backWidth:   backVideo.videoWidth,
                backHeight:  backVideo.videoHeight,
                duration:    duration
            });

            console.log("Front:", frontVideo.videoWidth, "x", frontVideo.videoHeight);
            console.log("Back: ", backVideo.videoWidth,  "x", backVideo.videoHeight);
            console.log("Duration:", duration);
        })
        .catch(function (error) {
            console.error(error);
            showError(error.message);
        });
}

function waitForMetadata(video) {
    return new Promise(function (resolve, reject) {
        if (video.readyState >= 1) { resolve(); return; }

        video.addEventListener("loadedmetadata", resolve, { once: true });
        video.addEventListener("error", function () {
            reject(new Error("Unable to load video: " + video.src));
        }, { once: true });
    });
}

/*
 * ============================================================
 * Synchronization
 * ============================================================
 */

const DRIFT_OK          = 0.04;
const DRIFT_HARD_SEEK   = 0.25;
const CORRECTION_RATE   = 0.04;
const MIN_PLAYBACK_RATE = 0.96;
const MAX_PLAYBACK_RATE = 1.04;

function synchronizeVideos() {
    if (frontVideo.readyState < frontVideo.HAVE_CURRENT_DATA) return;
    if (backVideo.readyState  < backVideo.HAVE_CURRENT_DATA)  return;

    if (frontVideo.paused || backVideo.paused) {
        if (frontVideo.playbackRate !== 1.0) frontVideo.playbackRate = 1.0;
        if (backVideo.playbackRate  !== 1.0) backVideo.playbackRate  = 1.0;
        return;
    }

    const diff = backVideo.currentTime - frontVideo.currentTime;
    const abs  = Math.abs(diff);

    if (abs <= DRIFT_OK) {
        if (frontVideo.playbackRate !== 1.0) frontVideo.playbackRate = 1.0;
        if (backVideo.playbackRate  !== 1.0) backVideo.playbackRate  = 1.0;
        return;
    }

    if (abs >= DRIFT_HARD_SEEK) {
        if (diff > 0) frontVideo.currentTime = backVideo.currentTime;
        else          backVideo.currentTime  = frontVideo.currentTime;
        frontVideo.playbackRate = 1.0;
        backVideo.playbackRate  = 1.0;
        return;
    }

    if (diff > 0) {
        backVideo.playbackRate  = Math.max(MIN_PLAYBACK_RATE, 1.0 - CORRECTION_RATE);
        frontVideo.playbackRate = Math.min(MAX_PLAYBACK_RATE, 1.0 + CORRECTION_RATE);
    } else {
        frontVideo.playbackRate = Math.max(MIN_PLAYBACK_RATE, 1.0 - CORRECTION_RATE);
        backVideo.playbackRate  = Math.min(MAX_PLAYBACK_RATE, 1.0 + CORRECTION_RATE);
    }
}

/*
 * ============================================================
 * Play / pause
 * ============================================================
 */

async function togglePlayback() {
    if (frontVideo.paused) {
        try {
            frontVideo.playbackRate = 1.0;
            backVideo.playbackRate  = 1.0;

            await Promise.allSettled([
                frontVideo.play(),
                backVideo.play()
            ]);

            playButton.textContent = "❚❚";
            notifyQML("playingChanged", true);
        } catch (error) {
            console.error("Unable to play videos:", error);
            showError("Unable to play video.");
        }
    } else {
        frontVideo.pause();
        backVideo.pause();
        playButton.textContent = "▶";
        notifyQML("playingChanged", false);
    }
}

playButton.addEventListener("click", togglePlayback);

/*
 * ============================================================
 * Seeking
 * ============================================================
 */

progress.addEventListener("input", function () {
    const duration = frontVideo.duration;
    if (!isFinite(duration)) return;

    const time = parseFloat(progress.value) * duration;
    frontVideo.currentTime = time;
    backVideo.currentTime  = time;

    frontVideo.playbackRate = 1.0;
    backVideo.playbackRate  = 1.0;
});

/*
 * ============================================================
 * Fullscreen
 * ============================================================
 */

fullscreenButton.addEventListener("click", function () {
    if (!document.fullscreenElement) {
        document.getElementById("viewer").requestFullscreen();
    } else {
        document.exitFullscreen();
    }
});

/*
 * ============================================================
 * QML communication
 * ============================================================
 */

function notifyQML(method, argument) {
    if (!qmlBridge) return;
    try {
        if (argument === undefined) qmlBridge[method]();
        else                        qmlBridge[method](argument);
    } catch (error) {
        console.error("QML bridge error:", error);
    }
}

/*
 * ============================================================
 * Public API
 * ============================================================
 */

Object.assign(window.fusionViewer = {
    loadVideos(front, back) { setVideoSources(front, back); },
    play()  { if (frontVideo.paused)  togglePlayback(); },
    pause() { if (!frontVideo.paused) togglePlayback(); },
    togglePlayback() { togglePlayback(); },
    seek(seconds) {
        frontVideo.currentTime = seconds;
        backVideo.currentTime  = seconds;
    },
    setYaw(v)   { yaw = v; },
    setPitch(v) { pitch = v; },
    setFov(v)   { calibration.fov = v; },
    setBlendWidth(v) { calibration.blendWidth = v; },
    setGhostSharpness(v) { calibration.ghostSharpness = v; },
    setFrontRotation(v) { calibration.front.rotation = v; },
    setBackRotation(v)  { calibration.back.rotation  = v; },

    setFrontMirror(v) {
        calibration.front.mirror = v ? -1.0 : 1.0;
        syncMirrorCheckboxes();
    },
    setBackMirror(v) {
        calibration.back.mirror = v ? -1.0 : 1.0;
        syncMirrorCheckboxes();
    },

    /* ============================================
     * Profiles
     * ============================================ */

    /** Apply a stored profile by name. Returns true on success. */
    setProfile(name) {
        const profile = PROFILES[name];
        if (!profile) {
            console.warn("[viewer] unknown profile:", name);
            return false;
        }
        applyProfile(profile, name);
        notifyQML("profileChanged", { name: name });
        return true;
    },

    /** List available profile names. */
    getProfileNames() {
        return Object.keys(PROFILES);
    },

    /** Currently active profile name (may be null after raw setCalibration). */
    getProfileName() {
        return currentProfileName;
    },

    /** Register / overwrite a profile at runtime. */
    registerProfile(name, profile) {
        if (!name || !profile) return false;
        PROFILES[name] = cloneProfile(profile);
        return true;
    },

    /* ============================================
     * Raw calibration (unchanged semantics)
     * ============================================ */

    setExposure(front, back) {
        if (front > 0) { targetFrontExposure = currentFrontExposure = front; }
        if (back  > 0) { targetBackExposure  = currentBackExposure  = back;  }
    },
    setCalibration(config) {
        if (config.fov !== undefined)            calibration.fov = config.fov;
        if (config.blendWidth !== undefined)     calibration.blendWidth = config.blendWidth;
        if (config.ghostSharpness !== undefined) calibration.ghostSharpness = config.ghostSharpness;
        if (config.front) Object.assign(calibration.front, config.front);
        if (config.back)  Object.assign(calibration.back,  config.back);

        /* Detach from any named profile – the user hand-tuned it. */
        currentProfileName = null;
        initialCalibration = cloneProfile(calibration);

        syncAllSliders();
        syncMirrorCheckboxes();
    },
    getCalibration() {
        return JSON.parse(JSON.stringify(calibration));
    },
    resetView() { yaw = 0.0; pitch = 0.0; focal = 1.0; },

    getDrift() { return backVideo.currentTime - frontVideo.currentTime; },
    forceSync() {
        backVideo.currentTime  = frontVideo.currentTime;
        frontVideo.playbackRate = 1.0;
        backVideo.playbackRate  = 1.0;
    }
});

/* Mark ready and flush anything that was queued before */
window.fusionViewer._isReady = true;

if (window.fusionViewer._pendingVideos) {
    const [f, b] = window.fusionViewer._pendingVideos;
    window.fusionViewer._pendingVideos = null;
    setVideoSources(f, b);
}

if (window.fusionViewer._pendingPlay) {
    window.fusionViewer._pendingPlay = false;
    togglePlayback();
}

/*
 * ============================================================
 * Texture upload
 * ============================================================
 */

function updateTexture(texture, video) {
    if (video.readyState < video.HAVE_CURRENT_DATA) return;

    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, video);
}

/*
 * ============================================================
 * Rendering
 * ============================================================
 */

function render(timestamp) {
    resize();
    synchronizeVideos();
    updateExposureTargets(timestamp || 0);
    smoothExposure();

    updateTexture(frontTexture, frontVideo);
    updateTexture(backTexture,  backVideo);

    gl.useProgram(program);

    gl.uniform2f(uniforms.resolution, canvas.width, canvas.height);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, frontTexture);
    gl.uniform1i(uniforms.front, 0);

    gl.activeTexture(gl.TEXTURE1);
    gl.bindTexture(gl.TEXTURE_2D, backTexture);
    gl.uniform1i(uniforms.back, 1);

    gl.uniform1f(uniforms.fov, calibration.fov);

    gl.uniform2f(uniforms.frontCenter,
        calibration.front.center[0], calibration.front.center[1]);
    gl.uniform2f(uniforms.frontRadius,
        calibration.front.radius[0], calibration.front.radius[1]);

    gl.uniform2f(uniforms.backCenter,
        calibration.back.center[0], calibration.back.center[1]);
    gl.uniform2f(uniforms.backRadius,
        calibration.back.radius[0], calibration.back.radius[1]);

    gl.uniform1f(uniforms.frontRotation, calibration.front.rotation);
    gl.uniform1f(uniforms.backRotation,  calibration.back.rotation);

    gl.uniform1f(uniforms.frontMirror,   calibration.front.mirror);
    gl.uniform1f(uniforms.backMirror,    calibration.back.mirror);

    gl.uniform1f(uniforms.blendWidth,     calibration.blendWidth);
    gl.uniform1f(uniforms.ghostSharpness, calibration.ghostSharpness);
    gl.uniform1f(uniforms.frontExposure,  currentFrontExposure);
    gl.uniform1f(uniforms.backExposure,   currentBackExposure);

    gl.uniform1f(uniforms.yaw,   yaw);
    gl.uniform1f(uniforms.pitch, pitch);
    gl.uniform1f(uniforms.focal, focal);

    gl.uniform1f(uniforms.viewMode,  currentViewMode);
    gl.uniform1f(uniforms.showEdges, SHOW_CALIBRATION_PANEL ? currentShowEdges : 0.0);

    gl.drawArrays(gl.TRIANGLES, 0, 6);

    if (isFinite(frontVideo.duration) && frontVideo.duration > 0) {
        progress.value = frontVideo.currentTime / frontVideo.duration;
        timeLabel.textContent =
            formatTime(frontVideo.currentTime) + " / " + formatTime(frontVideo.duration);
    }

    requestAnimationFrame(render);
}

/*
 * ============================================================
 * Time formatting
 * ============================================================
 */

function formatTime(seconds) {
    if (!isFinite(seconds)) return "00:00";
    const m = Math.floor(seconds / 60);
    const s = Math.floor(seconds % 60);
    return String(m).padStart(2, "0") + ":" + String(s).padStart(2, "0");
}

/*
 * ============================================================
 * Errors
 * ============================================================
 */

function showError(message) {
    loading.textContent   = message;
    loading.style.display = "block";
    console.error(message);
}

/*
 * ============================================================
 * Start
 * ============================================================
 */

resize();

if (MOCK) {
    setVideoSources(frontVideoPath, backVideoPath);
}

requestAnimationFrame(render);