typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
//Typedefs are a way for us to create shorthands for typenames that are annoying to type every time.
//We are at 16:30 in the video. He's doing some more global definitions and whatnot to help make things static, since we are including files a lot.

#define global_variable static
#define internal static
//Set some more easy to understand variable declarations - makes it easy to mark globals or internal vars

inline int clamp(int min, int val, int max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}