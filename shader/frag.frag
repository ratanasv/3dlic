#version 430 compatibility

uniform sampler3D texToBeSliced;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;

void main(void) {
    gl_FragColor = texture(texToBeSliced, fTexCoord.stp);
}
