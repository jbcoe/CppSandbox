#pragma once

template <typename F_t>
class scoped_function
{
	F_t f;
public:
	scoped_function(F_t f_) : f(f_) {}
	~scoped_function() { f(); }
};

template <typename F_t>
scoped_function<F_t> make_scoped_function(F_t f)
{
	return scoped_function<F_t>(f);
}
