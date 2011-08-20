var SDL = require('../sdl'),
    GLES = SDL.GLES;

SDL.init(SDL.INIT.VIDEO);
var screen = SDL.setVideoMode(0,0,0,SDL.SURFACE.OPENGL);
SDL.events.on('QUIT', function () { process.exit(0); });

console.dir(GLES);
var Shaders = [
  GLES.createShader(GLES.VERTEX_SHADER),
  GLES.createShader(GLES.FRAGMENT_SHADER)
];

GLES.loadShader("\n\
  attribute vec3 Position;\n\
  attribute vec3 Normal;\n\
  uniform mat4 Proj;\n\
  uniform mat4 Model;\n\
  varying vec3 NormVec;\n\
  varying vec3 LighVec;\n\
  void main(void)\n\
  {\n\
    vec4 Pos = Model * vec4(Position, 1.0);\n\
    gl_Position = Proj * Pos;\n\
    NormVec     = (Model * vec4(Normal,0.0)).xyz;\n\
    LighVec     = -Pos.xyz;\n\
  }\n\
", Shaders[0]);

GLES.loadShader("\n\
  varying highp vec3 NormVec;\n\
  varying highp vec3 LighVec;\n\
  void main(void)\n\
  {\n\
    lowp vec3 Color = vec3(1.0, 0.0, 0.0);\n\
    mediump vec3 Norm  = normalize(NormVec);\n\
    mediump vec3 Light = normalize(LighVec);\n\
    mediump float Diffuse = dot(Norm, Light);\n\
    gl_FragColor = vec4(Color * (max(Diffuse, 0.0) * 0.6 + 0.4), 1.0);\n\
  }\n\
", Shaders[1]);

var Program = GLES.createProgram();

GLES.attachShader(Program, Shaders[0]);
GLES.attachShader(Program, Shaders[1]);

GLES.bindAttribLocation(Program, 0, "Position");
GLES.bindAttribLocation(Program, 1, "Normal");

GLES.linkProgram(Program);

// Retrieve our uniforms
iProj   = GLES.getUniformLocation(Program, "Proj");
iModel  = GLES.getUniformLocation(Program, "Model");

// Basic GL setup
GLES.clearColor(0.0, 0.0, 0.0, 1.0);
GLES.enable(GLES.CULL_FACE);
GLES.cullFace(GLES.BACK);


console.dir({iProj:iProj,iModel:iModel,Shaders: Shaders, Program: Program});

//GLES.doit();


