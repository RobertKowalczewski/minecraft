#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform float left;
uniform float top;
uniform float xside;
uniform float yside;

uniform int res_x;
uniform int res_y;

uniform int iterations;

void main() {
	float xscale, yscale, zx, zy, cx, tempx, cy;
	int i, j;

	cx = TexCoord.x * xside + left;
	cy = TexCoord.y * yside + top;
	zx = 0;
	zy = 0;
	float count = 0;
	while ((zx * zx + zy * zy < 4) && (count < iterations)) {
		// tempx = z_real*_real - z_imaginary*z_imaginary + c_real
		tempx = zx * zx - zy * zy + cx;

		// 2*z_real*z_imaginary + c_imaginary
		zy = 2 * zx * zy + cy;

		// Updating z_real = tempx
		zx = tempx;

		// Increment count
		count = count + 1;
	}
	float c = count / float(iterations);
	FragColor = vec4(c, c, c, 1.);
}
