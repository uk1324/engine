#pragma once

template<typename Function, typename Scalar>
auto derivativeMidpoint(Function f, Scalar x, Scalar h) -> decltype(f(x)) {
	return (f(x + h) - f(x - h)) / (Scalar(2) * h);
}

template<typename Function, typename Scalar>
auto mixedDerivativeMidpoint(Function f, Scalar x, Scalar y, Scalar hx, Scalar hy) -> decltype(f(x, y)) {
	return (f(x + hx, y + hy) - f(x + hx, y - hy) - f(x - hx, y + hy) + f(x - hx, y - hy)) / (Scalar(4) * hx * hy);
}


// Could make it more general so that it can for example take a vector as the argument to the function, but then there would need to be another type added, the scalar type and also there would need to be a function to get the magnitiude of the step.
template<typename Function, typename Scalar>
auto secondDerivativeMidpoint(Function f, Scalar x, Scalar h) -> decltype(f(x)) {
	return (f(x - h) - Scalar(2) * f(x) + f(x + h)) / (h * h);
}