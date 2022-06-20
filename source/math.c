#include "main.h"

double lerp(double x, lerp_function func) {
	switch (func) {
		case LINEAR:
			return x;
		case EASE_IN_SINE:
			return 1 - cos((x * M_PI) / 2);
		case EASE_OUT_SINE:
			return sin((x * M_PI) / 2);
		case EASE_IN_OUT_SINE:
			return -(cos(M_PI * x) - 1) / 2;
		case EASE_IN_QUAD:
			return x * x;
		case EASE_OUT_QUAD:
			return 1 - (1 - x) * (1 - x);
		case EASE_IN_OUT_QUAD:
			return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
		case EASE_IN_CUBIC:
			return x * x * x;
		case EASE_OUT_CUBIC:
			return 1 - pow(1 - x, 3);
		case EASE_IN_OUT_CUBIC:
			return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
		case EASE_IN_QUART:
			return x * x * x * x;
		case EASE_OUT_QUART:
			return 1 - pow(1 - x, 4);
		case EASE_IN_OUT_QUART:
			return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
		case EASE_IN_QUINT:
			return x * x * x * x * x;
		case EASE_OUT_QUINT:
			return 1 - pow(1 - x, 5);
		case EASE_IN_OUT_QUINT:
			return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
		case EASE_IN_CIRC:
			return 1 - sqrt(1 - x * x);
		case EASE_OUT_CIRC:
			return sqrt(1 - pow(1 - x, 2));
		case EASE_IN_OUT_CIRC:
			return x < 0.5 ? (1 - sqrt(1 - 4 * x * x)) / 2 : (sqrt(1 - 4 * (x - 1) * (x - 1)) + 1) / 2;
		case EASE_IN_EXPO:
			return x == 0 ? 0 : pow(2, 10 * x - 10);
		case EASE_OUT_EXPO:
			return x == 1 ? 1 : 1 - pow(2, -10 * x);
		case EASE_IN_OUT_EXPO:
			return x == 0
			? 0 
			: x == 1
			? 1 
			: x < 0.5
			? pow(2, 20 * x - 10) / 2 
			: (2 - pow(2, -20 * x + 10)) / 2;
		default:
			return x;
	}
}