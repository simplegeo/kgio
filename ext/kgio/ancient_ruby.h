#ifndef MISSING_ANCIENT_RUBY_H
#define MISSING_ANCIENT_RUBY_H

#ifndef HAVE_RB_STR_SET_LEN
static void rb_str_set_len(VALUE str, long len)
{
	RSTRING(str)->len = len;
	RSTRING(str)->ptr[len] = '\0';
}
#endif /* ! HAVE_RB_STR_SET_LEN */

#ifndef RSTRING_PTR
#  define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif /* !defined(RSTRING_PTR) */
#ifndef RSTRING_LEN
#  define RSTRING_LEN(s) (RSTRING(s)->len)
#endif /* !defined(RSTRING_LEN) */

#endif /* MISSING_ANCIENT_RUBY_H */
