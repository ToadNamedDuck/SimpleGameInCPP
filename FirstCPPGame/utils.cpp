typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static
//Set some more easy to understand variable declarations - makes it easy to mark globals or internal vars

inline int clamp(int min, int val, int max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}