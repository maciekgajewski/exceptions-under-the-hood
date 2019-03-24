void baz(bool);

void foo(bool t) noexcept {
  if (t)
    baz(t);
}
