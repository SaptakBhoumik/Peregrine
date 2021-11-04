double _PEREGRINE_POWER(double base, double power) {
  double result = 1;
  if (power >= 0) {

    while (power--) {
      result = result * base;
    }

    return result;
  } else {
    while (power++) {
      result = result * base;
    }
    if(base!=0)
      return 1 / result;
    else return 0;
  }
}