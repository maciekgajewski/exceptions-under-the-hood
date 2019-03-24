void baz(bool);

void foo(bool t) {
  if (t)
    baz(t);
}
