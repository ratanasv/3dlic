#version 430 compatbility

uniform sampler3D texToBeSliced;

in vec3 fTexCoord;
in vec3 fColor;
in vec3 fNormal;

void main(void) {
    gl_FragColor = Texture3D(texToBeSliced, fTexCoord.stp);
}
