void foo(bool) noexcept;
void bar(bool t){
	if (t) foo(t);
}

