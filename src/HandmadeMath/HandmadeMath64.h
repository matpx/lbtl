/*
  HandmadeMath.h v2.0.0

  This is a single header file with a bunch of useful types and functions for
  games and graphics. Consider it a lightweight alternative to GLM that works
  both C and C++.

  =============================================================================
  CONFIG
  =============================================================================

  By default, all angles in Handmade Math are specified in radians. However, it
  can be configured to use degrees or turns instead. Use one of the following
  defines to specify the default unit for angles:

    #define HANDMADE_MATH_USE_RADIANS
    #define HANDMADE_MATH_USE_DEGREES
    #define HANDMADE_MATH_USE_TURNS

  Regardless of the default angle, you can use the following functions to
  specify an angle in a particular unit:

    HMMD_AngleRad(radians)
    HMMD_AngleDeg(degrees)
    HMMD_AngleTurn(turns)

  The definitions of these functions change depending on the default unit.

  -----------------------------------------------------------------------------

  Handmade Math ships with SSE (SIMD) implementations of several common
  operations. To disable the use of SSE intrinsics, you must define
  HANDMADE_MATH_NO_SSE before including this file:

    #define HANDMADE_MATH_NO_SSE
    #include "HandmadeMath.h"

  -----------------------------------------------------------------------------

  To use Handmade Math without the C runtime library, you must provide your own
  implementations of basic math functions. Otherwise, HandmadeMath.h will use
  the runtime library implementation of these functions.

  Define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS and provide your own
  implementations of HMMD_SINF, HMMD_COSF, HMMD_TANF, HMMD_ACOSF, and HMMD_SQRTF
  before including HandmadeMath.h, like so:

    #define HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS
    #define HMMD_SINF MySinF
    #define HMMD_COSF MyCosF
    #define HMMD_TANF MyTanF
    #define HMMD_ACOSF MyACosF
    #define HMMD_SQRTF MySqrtF
    #include "HandmadeMath.h"
  
  By default, it is assumed that your math functions take radians. To use
  different units, you must define HMMD_ANGLE_USER_TO_INTERNAL and
  HMMD_ANGLE_INTERNAL_TO_USER. For example, if you want to use degrees in your
  code but your math functions use turns:

    #define HMMD_ANGLE_USER_TO_INTERNAL(a) ((a)*HMMD_DegToTurn)
    #define HMMD_ANGLE_INTERNAL_TO_USER(a) ((a)*HMMD_TurnToDeg)

  =============================================================================
  
  LICENSE

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.

  =============================================================================

  CREDITS

  Originally written by Zakary Strange.

  Functionality:
   Zakary Strange (strangezak@protonmail.com && @strangezak)
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness)
   Trinton Bullard (@Peliex_Dev)
   @AntonDan
   Logan Forman (@dev_dwarf)

  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

// Dummy macros for when test framework is not present.
#ifndef COVERAGE
# define COVERAGE(a, b)
#endif

#ifndef ASSERT_COVERED
# define ASSERT_COVERED(a)
#endif

/* let's figure out if SSE is really available (unless disabled anyway)
   (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
   => only use "#ifdef HANDMADE_MATH__USE_SSE" to check for SSE support below this block! */
#ifndef HANDMADE_MATH_NO_SSE
# ifdef _MSC_VER /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
#  if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#   define HANDMADE_MATH__USE_SSE 1
#  endif
# else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
#  ifdef __SSE__ /* they #define __SSE__ if it's supported */
#   define HANDMADE_MATH__USE_SSE 1
#  endif /*  __SSE__ */
# endif /* not _MSC_VER */
#endif /* #ifndef HANDMADE_MATH_NO_SSE */

#if (!defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
# define HANDMADE_MATH__USE_C11_GENERICS 1
#endif

#ifdef HANDMADE_MATH__USE_SSE
# include <xmmintrin.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wfloat-equal"
# if (defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)) || defined(__clang__)
#  pragma GCC diagnostic ignored "-Wmissing-braces"
# endif
# ifdef __clang__
#  pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
# endif
#endif

#if defined(__GNUC__) || defined(__clang__)
# define HMMD_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
# define HMMD_DEPRECATED(msg) __declspec(deprecated(msg))
#else
# define HMMD_DEPRECATED(msg)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HANDMADE_MATH_USE_DEGREES) \
    && !defined(HANDMADE_MATH_USE_TURNS) \
    && !defined(HANDMADE_MATH_USE_RADIANS)
# define HANDMADE_MATH_USE_RADIANS
#endif
    
#define HMMD_PI 3.14159265358979323846
#define HMMD_PI32 3.14159265359f
#define HMMD_DEG180 180.0
#define HMMD_DEG18032 180.0f
#define HMMD_TURNHALF 0.5
#define HMMD_TURNHALF32 0.5f
#define HMMD_RadToDeg ((double)(HMMD_DEG180/HMMD_PI))
#define HMMD_RadToTurn ((double)(HMMD_TURNHALF/HMMD_PI))
#define HMMD_DegToRad ((double)(HMMD_PI/HMMD_DEG180))
#define HMMD_DegToTurn ((double)(HMMD_TURNHALF/HMMD_DEG180))
#define HMMD_TurnToRad ((double)(HMMD_PI/HMMD_TURNHALF))
#define HMMD_TurnToDeg ((double)(HMMD_DEG180/HMMD_TURNHALF))

#if defined(HANDMADE_MATH_USE_RADIANS)
# define HMMD_AngleRad(a) (a)
# define HMMD_AngleDeg(a) ((a)*HMMD_DegToRad)
# define HMMD_AngleTurn(a) ((a)*HMMD_TurnToRad)
#elif defined(HANDMADE_MATH_USE_DEGREES)
# define HMMD_AngleRad(a) ((a)*HMMD_RadToDeg)
# define HMMD_AngleDeg(a) (a)
# define HMMD_AngleTurn(a) ((a)*HMMD_TurnToDeg)
#elif defined(HANDMADE_MATH_USE_TURNS)
# define HMMD_AngleRad(a) ((a)*HMMD_RadToTurn)
# define HMMD_AngleDeg(a) ((a)*HMMD_DegToTurn)
# define HMMD_AngleTurn(a) (a)
#endif

#if !defined(HANDMADE_MATH_PROVIDE_MATH_FUNCTIONS)
# include <math.h>
# define HMMD_SINF sinf
# define HMMD_COSF cosf
# define HMMD_TANF tanf
# define HMMD_SQRTF sqrtf
# define HMMD_ACOSF acosf
#endif

#if !defined(HMMD_ANGLE_USER_TO_INTERNAL)
# define HMMD_ANGLE_USER_TO_INTERNAL(a) (HMMD_ToRad(a))
#endif

#if !defined(HMMD_ANGLE_INTERNAL_TO_USER)
# if defined(HANDMADE_MATH_USE_RADIANS)
#  define HMMD_ANGLE_INTERNAL_TO_USER(a) (a) 
# elif defined(HANDMADE_MATH_USE_DEGREES)
#  define HMMD_ANGLE_INTERNAL_TO_USER(a) ((a)*HMMD_RadToDeg)
# elif defined(HANDMADE_MATH_USE_TURNS)
#  define HMMD_ANGLE_INTERNAL_TO_USER(a) ((a)*HMMD_RadToTurn)
# endif
#endif

#define HMMD_MIN(a, b) ((a) > (b) ? (b) : (a))
#define HMMD_MAX(a, b) ((a) < (b) ? (b) : (a))
#define HMMD_ABS(a) ((a) > 0 ? (a) : -(a))
#define HMMD_MOD(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define HMMD_SQUARE(x) ((x) * (x))

typedef union HMMD_Vec2
{
    struct
    {
        double X, Y;
    };

    struct
    {
        double U, V;
    };

    struct
    {
        double Left, Right;
    };

    struct
    {
        double Width, Height;
    };

    double Elements[2];

#ifdef __cplusplus
    inline double &operator[](int Index)
    {
        return Elements[Index];
    }
#endif
} HMMD_Vec2;

typedef union HMMD_Vec3
{
    struct
    {
        double X, Y, Z;
    };

    struct
    {
        double U, V, W;
    };

    struct
    {
        double R, G, B;
    };

    struct
    {
        HMMD_Vec2 XY;
        double _Ignored0;
    };

    struct
    {
        double _Ignored1;
        HMMD_Vec2 YZ;
    };

    struct
    {
        HMMD_Vec2 UV;
        double _Ignored2;
    };

    struct
    {
        double _Ignored3;
        HMMD_Vec2 VW;
    };

    double Elements[3];

#ifdef __cplusplus
    inline double &operator[](int Index)
    {
        return Elements[Index];
    }
#endif
} HMMD_Vec3;

typedef union HMMD_Vec4
{
    struct
    {
        union
        {
            HMMD_Vec3 XYZ;
            struct
            {
                double X, Y, Z;
            };
        };

        double W;
    };
    struct
    {
        union
        {
            HMMD_Vec3 RGB;
            struct
            {
                double R, G, B;
            };
        };

        double A;
    };

    struct
    {
        HMMD_Vec2 XY;
        double _Ignored0;
        double _Ignored1;
    };

    struct
    {
        double _Ignored2;
        HMMD_Vec2 YZ;
        double _Ignored3;
    };

    struct
    {
        double _Ignored4;
        double _Ignored5;
        HMMD_Vec2 ZW;
    };

    double Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSE;
#endif

#ifdef __cplusplus
    inline double &operator[](int Index)
    {
        return Elements[Index];
    }
#endif
} HMMD_Vec4;

typedef union HMMD_Mat2
{
    double Elements[2][2];
    HMMD_Vec2 Columns[2];

#ifdef __cplusplus
    inline HMMD_Vec2 &operator[](int Index)
    {
        return Columns[Index];
    }
#endif
} HMMD_Mat2;
    
typedef union HMMD_Mat3
{
    double Elements[3][3];
    HMMD_Vec3 Columns[3];

#ifdef __cplusplus
    inline HMMD_Vec3 &operator[](int Index)
    {
        return Columns[Index];
    }
#endif
} HMMD_Mat3;

typedef union HMMD_Mat4
{
    double Elements[4][4];
    HMMD_Vec4 Columns[4];

#ifdef __cplusplus
    inline HMMD_Vec4 &operator[](int Index)
    {
        return Columns[Index];
    }
#endif
} HMMD_Mat4;

typedef union HMMD_Quat
{
    struct
    {
        union
        {
            HMMD_Vec3 XYZ;
            struct
            {
                double X, Y, Z;
            };
        };

        double W;
    };

    double Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSE;
#endif
} HMMD_Quat;

typedef signed int HMMD_Bool;

/*
 * Angle unit conversion functions
 */
static inline double HMMD_ToRad(double Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    double Result = Angle;
#elif defined(HANDMADE_MATH_USE_DEGREES) 
    double Result = Angle * HMMD_DegToRad;
#elif defined(HANDMADE_MATH_USE_TURNS)
    double Result = Angle * HMMD_TurnToRad;
#endif
    
    return Result;
}

static inline double HMMD_ToDeg(double Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    double Result = Angle * HMMD_RadToDeg;
#elif defined(HANDMADE_MATH_USE_DEGREES) 
    double Result = Angle;
#elif defined(HANDMADE_MATH_USE_TURNS)
    double Result = Angle * HMMD_TurnToDeg;
#endif
    
    return Result;
}

static inline double HMMD_ToTurn(double Angle)
{
#if defined(HANDMADE_MATH_USE_RADIANS)
    double Result = Angle * HMMD_RadToTurn;
#elif defined(HANDMADE_MATH_USE_DEGREES) 
    double Result = Angle * HMMD_DegToTurn;
#elif defined(HANDMADE_MATH_USE_TURNS)
    double Result = Angle;
#endif
    
    return Result;
}

/*
 * Floating-point math functions
 */

COVERAGE(HMMD_SinF, 1)
static inline double HMMD_SinF(double Angle)
{
    ASSERT_COVERED(HMMD_SinF);
    return HMMD_SINF(HMMD_ANGLE_USER_TO_INTERNAL(Angle));
}

COVERAGE(HMMD_CosF, 1)
static inline double HMMD_CosF(double Angle)
{
    ASSERT_COVERED(HMMD_CosF);
    return HMMD_COSF(HMMD_ANGLE_USER_TO_INTERNAL(Angle));
}

COVERAGE(HMMD_TanF, 1)
static inline double HMMD_TanF(double Angle)
{
    ASSERT_COVERED(HMMD_TanF);
    return HMMD_TANF(HMMD_ANGLE_USER_TO_INTERNAL(Angle));
}

COVERAGE(HMMD_ACosF, 1)
static inline double HMMD_ACosF(double Arg)
{
    ASSERT_COVERED(HMMD_ACosF);
    return HMMD_ANGLE_INTERNAL_TO_USER(HMMD_ACOSF(Arg));
}

COVERAGE(HMMD_SqrtF, 1)
static inline double HMMD_SqrtF(double Float)
{
    ASSERT_COVERED(HMMD_SqrtF);

    double Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 In = _mm_set_ss(Float);
    __m128 Out = _mm_sqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#else
    Result = HMMD_SQRTF(Float);
#endif

    return Result;
}

COVERAGE(HMMD_InvSqrtF, 1)
static inline double HMMD_InvSqrtF(double Float)
{
    ASSERT_COVERED(HMMD_InvSqrtF);

    double Result;

    Result = 1.0f/HMMD_SqrtF(Float);

    return Result;
}


/*
 * Utility functions
 */

COVERAGE(HMMD_Lerp, 1)
static inline double HMMD_Lerp(double A, double Time, double B)
{
    ASSERT_COVERED(HMMD_Lerp);
    return (1.0f - Time) * A + Time * B;
}

COVERAGE(HMMD_Clamp, 1)
static inline double HMMD_Clamp(double Min, double Value, double Max)
{
    ASSERT_COVERED(HMMD_Clamp);

    double Result = Value;

    if (Result < Min)
    {
        Result = Min;
    }

    if (Result > Max)
    {
        Result = Max;
    }

    return Result;
}


/*
 * Vector initialization
 */

COVERAGE(HMMD_V2, 1)
static inline HMMD_Vec2 HMMD_V2(double X, double Y)
{
    ASSERT_COVERED(HMMD_V2);

    HMMD_Vec2 Result;
    Result.X = X;
    Result.Y = Y;

    return Result;
}

COVERAGE(HMMD_V3, 1)
static inline HMMD_Vec3 HMMD_V3(double X, double Y, double Z)
{
    ASSERT_COVERED(HMMD_V3);

    HMMD_Vec3 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return Result;
}

COVERAGE(HMMD_V4, 1)
static inline HMMD_Vec4 HMMD_V4(double X, double Y, double Z, double W)
{
    ASSERT_COVERED(HMMD_V4);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(X, Y, Z, W);
#else
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
#endif

    return Result;
}

COVERAGE(HMMD_V4V, 1)
static inline HMMD_Vec4 HMMD_V4V(HMMD_Vec3 Vector, double W)
{
    ASSERT_COVERED(HMMD_V4V);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(Vector.X, Vector.Y, Vector.Z, W);
#else
    Result.XYZ = Vector;
    Result.W = W;
#endif

    return Result;
}


/*
 * Binary vector operations
 */

COVERAGE(HMMD_AddV2, 1)
static inline HMMD_Vec2 HMMD_AddV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_AddV2);

    HMMD_Vec2 Result;
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;

    return Result;
}

COVERAGE(HMMD_AddV3, 1)
static inline HMMD_Vec3 HMMD_AddV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_AddV3);

    HMMD_Vec3 Result;
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;

    return Result;
}

COVERAGE(HMMD_AddV4, 1)
static inline HMMD_Vec4 HMMD_AddV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_AddV4);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_add_ps(Left.SSE, Right.SSE);
#else
    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_SubV2, 1)
static inline HMMD_Vec2 HMMD_SubV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_SubV2);

    HMMD_Vec2 Result;
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;

    return Result;
}

COVERAGE(HMMD_SubV3, 1)
static inline HMMD_Vec3 HMMD_SubV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_SubV3);

    HMMD_Vec3 Result;
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;

    return Result;
}

COVERAGE(HMMD_SubV4, 1)
static inline HMMD_Vec4 HMMD_SubV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_SubV4);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_sub_ps(Left.SSE, Right.SSE);
#else
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_MulV2, 1)
static inline HMMD_Vec2 HMMD_MulV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_MulV2);

    HMMD_Vec2 Result;
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;

    return Result;
}

COVERAGE(HMMD_MulV2F, 1)
static inline HMMD_Vec2 HMMD_MulV2F(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV2F);

    HMMD_Vec2 Result;
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;

    return Result;
}

COVERAGE(HMMD_MulV3, 1)
static inline HMMD_Vec3 HMMD_MulV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_MulV3);

    HMMD_Vec3 Result;
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;

    return Result;
}

COVERAGE(HMMD_MulV3F, 1)
static inline HMMD_Vec3 HMMD_MulV3F(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV3F);

    HMMD_Vec3 Result;
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;
    Result.Z = Left.Z * Right;

    return Result;
}

COVERAGE(HMMD_MulV4, 1)
static inline HMMD_Vec4 HMMD_MulV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_MulV4);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_mul_ps(Left.SSE, Right.SSE);
#else
    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;
    Result.W = Left.W * Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_MulV4F, 1)
static inline HMMD_Vec4 HMMD_MulV4F(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV4F);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Right);
    Result.SSE = _mm_mul_ps(Left.SSE, Scalar);
#else
    Result.X = Left.X * Right;
    Result.Y = Left.Y * Right;
    Result.Z = Left.Z * Right;
    Result.W = Left.W * Right;
#endif

    return Result;
}

COVERAGE(HMMD_DivV2, 1)
static inline HMMD_Vec2 HMMD_DivV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_DivV2);

    HMMD_Vec2 Result;
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;

    return Result;
}

COVERAGE(HMMD_DivV2F, 1)
static inline HMMD_Vec2 HMMD_DivV2F(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV2F);

    HMMD_Vec2 Result;
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;

    return Result;
}

COVERAGE(HMMD_DivV3, 1)
static inline HMMD_Vec3 HMMD_DivV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_DivV3);

    HMMD_Vec3 Result;
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;

    return Result;
}

COVERAGE(HMMD_DivV3F, 1)
static inline HMMD_Vec3 HMMD_DivV3F(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV3F);

    HMMD_Vec3 Result;
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;
    Result.Z = Left.Z / Right;

    return Result;
}

COVERAGE(HMMD_DivV4, 1)
static inline HMMD_Vec4 HMMD_DivV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_DivV4);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_div_ps(Left.SSE, Right.SSE);
#else
    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;
    Result.W = Left.W / Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_DivV4F, 1)
static inline HMMD_Vec4 HMMD_DivV4F(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV4F);

    HMMD_Vec4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Right);
    Result.SSE = _mm_div_ps(Left.SSE, Scalar);
#else
    Result.X = Left.X / Right;
    Result.Y = Left.Y / Right;
    Result.Z = Left.Z / Right;
    Result.W = Left.W / Right;
#endif

    return Result;
}

COVERAGE(HMMD_EqV2, 1)
static inline HMMD_Bool HMMD_EqV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_EqV2);
    return Left.X == Right.X && Left.Y == Right.Y;
}

COVERAGE(HMMD_EqV3, 1)
static inline HMMD_Bool HMMD_EqV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_EqV3);
    return Left.X == Right.X && Left.Y == Right.Y && Left.Z == Right.Z;
}

COVERAGE(HMMD_EqV4, 1)
static inline HMMD_Bool HMMD_EqV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_EqV4);
    return Left.X == Right.X && Left.Y == Right.Y && Left.Z == Right.Z && Left.W == Right.W;
}

COVERAGE(HMMD_DotV2, 1)
static inline double HMMD_DotV2(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_DotV2);
    return (Left.X * Right.X) + (Left.Y * Right.Y);
}

COVERAGE(HMMD_DotV3, 1)
static inline double HMMD_DotV3(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_DotV3);
    return (Left.X * Right.X) + (Left.Y * Right.Y) + (Left.Z * Right.Z);
}

COVERAGE(HMMD_DotV4, 1)
static inline double HMMD_DotV4(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_DotV4);

    double Result;

    // NOTE(zak): IN the future if we wanna check what version SSE is support
    // we can use _mm_dp_ps (4.3) but for now we will use the old way.
    // Or a r = _mm_mul_ps(v1, v2), r = _mm_hadd_ps(r, r), r = _mm_hadd_ps(r, r) for SSE3
#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, Right.SSE);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#else
    Result = ((Left.X * Right.X) + (Left.Z * Right.Z)) + ((Left.Y * Right.Y) + (Left.W * Right.W));
#endif

    return Result;
}

COVERAGE(HMMD_Cross, 1)
static inline HMMD_Vec3 HMMD_Cross(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_Cross);

    HMMD_Vec3 Result;
    Result.X = (Left.Y * Right.Z) - (Left.Z * Right.Y);
    Result.Y = (Left.Z * Right.X) - (Left.X * Right.Z);
    Result.Z = (Left.X * Right.Y) - (Left.Y * Right.X);

    return Result;
}


/*
 * Unary vector operations
 */

COVERAGE(HMMD_LenSqrV2, 1)
static inline double HMMD_LenSqrV2(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_LenSqrV2);
    return HMMD_DotV2(A, A);
}

COVERAGE(HMMD_LenSqrV3, 1)
static inline double HMMD_LenSqrV3(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_LenSqrV3);
    return HMMD_DotV3(A, A);
}

COVERAGE(HMMD_LenSqrV4, 1)
static inline double HMMD_LenSqrV4(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_LenSqrV4);
    return HMMD_DotV4(A, A);
}

COVERAGE(HMMD_LenV2, 1)
static inline double HMMD_LenV2(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_LenV2);
    return HMMD_SqrtF(HMMD_LenSqrV2(A));
}

COVERAGE(HMMD_LenV3, 1)
static inline double HMMD_LenV3(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_LenV3);
    return HMMD_SqrtF(HMMD_LenSqrV3(A));
}

COVERAGE(HMMD_LenV4, 1)
static inline double HMMD_LenV4(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_LenV4);
    return HMMD_SqrtF(HMMD_LenSqrV4(A));
}

COVERAGE(HMMD_NormV2, 1)
static inline HMMD_Vec2 HMMD_NormV2(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_NormV2);
    return HMMD_MulV2F(A, HMMD_InvSqrtF(HMMD_DotV2(A, A)));
}

COVERAGE(HMMD_NormV3, 1)
static inline HMMD_Vec3 HMMD_NormV3(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_NormV3);
    return HMMD_MulV3F(A, HMMD_InvSqrtF(HMMD_DotV3(A, A)));
}

COVERAGE(HMMD_NormV4, 1)
static inline HMMD_Vec4 HMMD_NormV4(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_NormV4);
    return HMMD_MulV4F(A, HMMD_InvSqrtF(HMMD_DotV4(A, A)));
}

/*
 * Utility vector functions
 */

COVERAGE(HMMD_LerpV2, 1)
static inline HMMD_Vec2 HMMD_LerpV2(HMMD_Vec2 A, double Time, HMMD_Vec2 B) 
{
    ASSERT_COVERED(HMMD_LerpV2);
    return HMMD_AddV2(HMMD_MulV2F(A, 1.0f - Time), HMMD_MulV2F(B, Time));
}

COVERAGE(HMMD_LerpV3, 1)
static inline HMMD_Vec3 HMMD_LerpV3(HMMD_Vec3 A, double Time, HMMD_Vec3 B) 
{
    ASSERT_COVERED(HMMD_LerpV3);
    return HMMD_AddV3(HMMD_MulV3F(A, 1.0f - Time), HMMD_MulV3F(B, Time));
}

COVERAGE(HMMD_LerpV4, 1)
static inline HMMD_Vec4 HMMD_LerpV4(HMMD_Vec4 A, double Time, HMMD_Vec4 B) 
{
    ASSERT_COVERED(HMMD_LerpV4);
    return HMMD_AddV4(HMMD_MulV4F(A, 1.0f - Time), HMMD_MulV4F(B, Time));
}

/*
 * SSE stuff
 */

COVERAGE(HMMD_LinearCombineV4M4, 1)
static inline HMMD_Vec4 HMMD_LinearCombineV4M4(HMMD_Vec4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_LinearCombineV4M4);

    HMMD_Vec4 Result;
#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0x00), Right.Columns[0].SSE);
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0x55), Right.Columns[1].SSE));
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0xaa), Right.Columns[2].SSE));
    Result.SSE = _mm_add_ps(Result.SSE, _mm_mul_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, 0xff), Right.Columns[3].SSE));
#else
    Result.X = Left.Elements[0] * Right.Columns[0].X;
    Result.Y = Left.Elements[0] * Right.Columns[0].Y;
    Result.Z = Left.Elements[0] * Right.Columns[0].Z;
    Result.W = Left.Elements[0] * Right.Columns[0].W;

    Result.X += Left.Elements[1] * Right.Columns[1].X;
    Result.Y += Left.Elements[1] * Right.Columns[1].Y;
    Result.Z += Left.Elements[1] * Right.Columns[1].Z;
    Result.W += Left.Elements[1] * Right.Columns[1].W;

    Result.X += Left.Elements[2] * Right.Columns[2].X;
    Result.Y += Left.Elements[2] * Right.Columns[2].Y;
    Result.Z += Left.Elements[2] * Right.Columns[2].Z;
    Result.W += Left.Elements[2] * Right.Columns[2].W;

    Result.X += Left.Elements[3] * Right.Columns[3].X;
    Result.Y += Left.Elements[3] * Right.Columns[3].Y;
    Result.Z += Left.Elements[3] * Right.Columns[3].Z;
    Result.W += Left.Elements[3] * Right.Columns[3].W;
#endif

    return Result;
}

/*
 * 2x2 Matrices
 */

COVERAGE(HMMD_M2, 1)
static inline HMMD_Mat2 HMMD_M2(void)
{
    ASSERT_COVERED(HMMD_M2);
    HMMD_Mat2 Result = {0};
    return Result;
}

COVERAGE(HMMD_M2D, 1)
static inline HMMD_Mat2 HMMD_M2D(double Diagonal)
{
    ASSERT_COVERED(HMMD_M2D);
    
    HMMD_Mat2 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;

    return Result;
}

COVERAGE(HMMD_TransposeM2, 1)
static inline HMMD_Mat2 HMMD_TransposeM2(HMMD_Mat2 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM2);
    
    HMMD_Mat2 Result = Matrix;

    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];
    
    return Result;
}

COVERAGE(HMMD_AddM2, 1)
static inline HMMD_Mat2 HMMD_AddM2(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_AddM2);
    
    HMMD_Mat2 Result;

    Result.Elements[0][0] = Left.Elements[0][0] + Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] + Right.Elements[0][1];
    Result.Elements[1][0] = Left.Elements[1][0] + Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] + Right.Elements[1][1];
   
    return Result;    
}

COVERAGE(HMMD_SubM2, 1)
static inline HMMD_Mat2 HMMD_SubM2(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_SubM2);
    
    HMMD_Mat2 Result;

    Result.Elements[0][0] = Left.Elements[0][0] - Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] - Right.Elements[0][1];
    Result.Elements[1][0] = Left.Elements[1][0] - Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] - Right.Elements[1][1];
    
    return Result;
}

COVERAGE(HMMD_MulM2V2, 1)
static inline HMMD_Vec2 HMMD_MulM2V2(HMMD_Mat2 Matrix, HMMD_Vec2 Vector)
{
    ASSERT_COVERED(HMMD_MulM2V2);
    
    HMMD_Vec2 Result;

    Result.X = Vector.Elements[0] * Matrix.Columns[0].X;
    Result.Y = Vector.Elements[0] * Matrix.Columns[0].Y;

    Result.X += Vector.Elements[1] * Matrix.Columns[1].X;
    Result.Y += Vector.Elements[1] * Matrix.Columns[1].Y;

    return Result;    
}

COVERAGE(HMMD_MulM2, 1)
static inline HMMD_Mat2 HMMD_MulM2(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_MulM2);
    
    HMMD_Mat2 Result;
    Result.Columns[0] = HMMD_MulM2V2(Left, Right.Columns[0]);
    Result.Columns[1] = HMMD_MulM2V2(Left, Right.Columns[1]);

    return Result;    
}

COVERAGE(HMMD_MulM2F, 1)
static inline HMMD_Mat2 HMMD_MulM2F(HMMD_Mat2 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_MulM2F);
    
    HMMD_Mat2 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;
    
    return Result;
}

COVERAGE(HMMD_DivM2F, 1)
static inline HMMD_Mat2 HMMD_DivM2F(HMMD_Mat2 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_DivM2F);
    
    HMMD_Mat2 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;

    return Result;
}

COVERAGE(HMMD_DeterminantM2, 1)
static inline double HMMD_DeterminantM2(HMMD_Mat2 Matrix) 
{
    ASSERT_COVERED(HMMD_DeterminantM2);
    return Matrix.Elements[0][0]*Matrix.Elements[1][1] - Matrix.Elements[0][1]*Matrix.Elements[1][0];
}


COVERAGE(HMMD_InvGeneralM2, 1)
static inline HMMD_Mat2 HMMD_InvGeneralM2(HMMD_Mat2 Matrix) 
{
    ASSERT_COVERED(HMMD_InvGeneralM2);

    HMMD_Mat2 Result;
    double InvDeterminant = 1.0f / HMMD_DeterminantM2(Matrix);
    Result.Elements[0][0] = InvDeterminant * +Matrix.Elements[1][1];
    Result.Elements[1][1] = InvDeterminant * +Matrix.Elements[0][0];
    Result.Elements[0][1] = InvDeterminant * -Matrix.Elements[0][1];
    Result.Elements[1][0] = InvDeterminant * -Matrix.Elements[1][0];

    return Result;
}

/*
 * 3x3 Matrices
 */

COVERAGE(HMMD_M3, 1)
static inline HMMD_Mat3 HMMD_M3(void)
{
    ASSERT_COVERED(HMMD_M3);
    HMMD_Mat3 Result = {0};
    return Result;
}

COVERAGE(HMMD_M3D, 1)
static inline HMMD_Mat3 HMMD_M3D(double Diagonal)
{
    ASSERT_COVERED(HMMD_M3D);
    
    HMMD_Mat3 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;

    return Result;
}

COVERAGE(HMMD_TransposeM3, 1)
static inline HMMD_Mat3 HMMD_TransposeM3(HMMD_Mat3 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM3);

    HMMD_Mat3 Result = Matrix;

    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[0][2] = Matrix.Elements[2][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];
    Result.Elements[1][2] = Matrix.Elements[2][1];
    Result.Elements[2][1] = Matrix.Elements[1][2];
    Result.Elements[2][0] = Matrix.Elements[0][2];
    
    return Result;
}

COVERAGE(HMMD_AddM3, 1)
static inline HMMD_Mat3 HMMD_AddM3(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_AddM3);
    
    HMMD_Mat3 Result;
    
    Result.Elements[0][0] = Left.Elements[0][0] + Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] + Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] + Right.Elements[0][2];
    Result.Elements[1][0] = Left.Elements[1][0] + Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] + Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] + Right.Elements[1][2];
    Result.Elements[2][0] = Left.Elements[2][0] + Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] + Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] + Right.Elements[2][2];

    return Result;    
}

COVERAGE(HMMD_SubM3, 1)
static inline HMMD_Mat3 HMMD_SubM3(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_SubM3);

    HMMD_Mat3 Result;

    Result.Elements[0][0] = Left.Elements[0][0] - Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] - Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] - Right.Elements[0][2];
    Result.Elements[1][0] = Left.Elements[1][0] - Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] - Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] - Right.Elements[1][2];
    Result.Elements[2][0] = Left.Elements[2][0] - Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] - Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] - Right.Elements[2][2];

    return Result;
}

COVERAGE(HMMD_MulM3V3, 1)
static inline HMMD_Vec3 HMMD_MulM3V3(HMMD_Mat3 Matrix, HMMD_Vec3 Vector)
{
    ASSERT_COVERED(HMMD_MulM3V3);
    
    HMMD_Vec3 Result;

    Result.X = Vector.Elements[0] * Matrix.Columns[0].X;
    Result.Y = Vector.Elements[0] * Matrix.Columns[0].Y;
    Result.Z = Vector.Elements[0] * Matrix.Columns[0].Z;

    Result.X += Vector.Elements[1] * Matrix.Columns[1].X;
    Result.Y += Vector.Elements[1] * Matrix.Columns[1].Y;
    Result.Z += Vector.Elements[1] * Matrix.Columns[1].Z;

    Result.X += Vector.Elements[2] * Matrix.Columns[2].X;
    Result.Y += Vector.Elements[2] * Matrix.Columns[2].Y;
    Result.Z += Vector.Elements[2] * Matrix.Columns[2].Z;
    
    return Result;    
}

COVERAGE(HMMD_MulM3, 1)
static inline HMMD_Mat3 HMMD_MulM3(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_MulM3);

    HMMD_Mat3 Result;
    Result.Columns[0] = HMMD_MulM3V3(Left, Right.Columns[0]);
    Result.Columns[1] = HMMD_MulM3V3(Left, Right.Columns[1]);
    Result.Columns[2] = HMMD_MulM3V3(Left, Right.Columns[2]);

    return Result;    
}

COVERAGE(HMMD_MulM3F, 1)
static inline HMMD_Mat3 HMMD_MulM3F(HMMD_Mat3 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_MulM3F);

    HMMD_Mat3 Result;

    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] * Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] * Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] * Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] * Scalar;

    return Result;            
}

COVERAGE(HMMD_DivM3, 1)
static inline HMMD_Mat3 HMMD_DivM3F(HMMD_Mat3 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_DivM3);

    HMMD_Mat3 Result;
    
    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] / Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] / Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] / Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] / Scalar;

    return Result;                    
}

COVERAGE(HMMD_DeterminantM3, 1)
static inline double HMMD_DeterminantM3(HMMD_Mat3 Matrix) 
{
    ASSERT_COVERED(HMMD_DeterminantM3);

    HMMD_Mat3 Cross;
    Cross.Columns[0] = HMMD_Cross(Matrix.Columns[1], Matrix.Columns[2]);
    Cross.Columns[1] = HMMD_Cross(Matrix.Columns[2], Matrix.Columns[0]);
    Cross.Columns[2] = HMMD_Cross(Matrix.Columns[0], Matrix.Columns[1]);

    return HMMD_DotV3(Cross.Columns[2], Matrix.Columns[2]);
}

COVERAGE(HMMD_InvGeneralM3, 1)
static inline HMMD_Mat3 HMMD_InvGeneralM3(HMMD_Mat3 Matrix) 
{
    ASSERT_COVERED(HMMD_InvGeneralM3);

    HMMD_Mat3 Cross;
    Cross.Columns[0] = HMMD_Cross(Matrix.Columns[1], Matrix.Columns[2]);
    Cross.Columns[1] = HMMD_Cross(Matrix.Columns[2], Matrix.Columns[0]);
    Cross.Columns[2] = HMMD_Cross(Matrix.Columns[0], Matrix.Columns[1]);

    double InvDeterminant = 1.0f / HMMD_DotV3(Cross.Columns[2], Matrix.Columns[2]);

    HMMD_Mat3 Result;
    Result.Columns[0] = HMMD_MulV3F(Cross.Columns[0], InvDeterminant);
    Result.Columns[1] = HMMD_MulV3F(Cross.Columns[1], InvDeterminant);
    Result.Columns[2] = HMMD_MulV3F(Cross.Columns[2], InvDeterminant);

    return HMMD_TransposeM3(Result);
}

/*
 * 4x4 Matrices
 */

COVERAGE(HMMD_M4, 1)
static inline HMMD_Mat4 HMMD_M4(void)
{
    ASSERT_COVERED(HMMD_M4);
    HMMD_Mat4 Result = {0};
    return Result;
}

COVERAGE(HMMD_M4D, 1)
static inline HMMD_Mat4 HMMD_M4D(double Diagonal)
{
    ASSERT_COVERED(HMMD_M4D);

    HMMD_Mat4 Result = {0};
    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;
    Result.Elements[3][3] = Diagonal;

    return Result;
}

COVERAGE(HMMD_TransposeM4, 1)
static inline HMMD_Mat4 HMMD_TransposeM4(HMMD_Mat4 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM4);

    HMMD_Mat4 Result = Matrix;
#ifdef HANDMADE_MATH__USE_SSE
    _MM_TRANSPOSE4_PS(Result.Columns[0].SSE, Result.Columns[1].SSE, Result.Columns[2].SSE, Result.Columns[3].SSE);
#else
    Result.Elements[0][1] = Matrix.Elements[1][0];
    Result.Elements[0][2] = Matrix.Elements[2][0];
    Result.Elements[0][3] = Matrix.Elements[3][0];
    Result.Elements[1][0] = Matrix.Elements[0][1];
    Result.Elements[1][2] = Matrix.Elements[2][1];
    Result.Elements[1][3] = Matrix.Elements[3][1];
    Result.Elements[2][1] = Matrix.Elements[1][2];
    Result.Elements[2][0] = Matrix.Elements[0][2];
    Result.Elements[2][3] = Matrix.Elements[3][2];
    Result.Elements[3][1] = Matrix.Elements[1][3];
    Result.Elements[3][2] = Matrix.Elements[2][3];
    Result.Elements[3][0] = Matrix.Elements[0][3];
#endif

    return Result;
}

COVERAGE(HMMD_AddM4, 1)
static inline HMMD_Mat4 HMMD_AddM4(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_AddM4);

    HMMD_Mat4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.Columns[0].SSE = _mm_add_ps(Left.Columns[0].SSE, Right.Columns[0].SSE);
    Result.Columns[1].SSE = _mm_add_ps(Left.Columns[1].SSE, Right.Columns[1].SSE);
    Result.Columns[2].SSE = _mm_add_ps(Left.Columns[2].SSE, Right.Columns[2].SSE);
    Result.Columns[3].SSE = _mm_add_ps(Left.Columns[3].SSE, Right.Columns[3].SSE);
#else
    Result.Elements[0][0] = Left.Elements[0][0] + Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] + Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] + Right.Elements[0][2];
    Result.Elements[0][3] = Left.Elements[0][3] + Right.Elements[0][3];
    Result.Elements[1][0] = Left.Elements[1][0] + Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] + Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] + Right.Elements[1][2];
    Result.Elements[1][3] = Left.Elements[1][3] + Right.Elements[1][3];
    Result.Elements[2][0] = Left.Elements[2][0] + Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] + Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] + Right.Elements[2][2];
    Result.Elements[2][3] = Left.Elements[2][3] + Right.Elements[2][3];
    Result.Elements[3][0] = Left.Elements[3][0] + Right.Elements[3][0];
    Result.Elements[3][1] = Left.Elements[3][1] + Right.Elements[3][1];
    Result.Elements[3][2] = Left.Elements[3][2] + Right.Elements[3][2];
    Result.Elements[3][3] = Left.Elements[3][3] + Right.Elements[3][3];
#endif

    return Result;
}

COVERAGE(HMMD_SubM4, 1)
static inline HMMD_Mat4 HMMD_SubM4(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_SubM4);

    HMMD_Mat4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.Columns[0].SSE = _mm_sub_ps(Left.Columns[0].SSE, Right.Columns[0].SSE);
    Result.Columns[1].SSE = _mm_sub_ps(Left.Columns[1].SSE, Right.Columns[1].SSE);
    Result.Columns[2].SSE = _mm_sub_ps(Left.Columns[2].SSE, Right.Columns[2].SSE);
    Result.Columns[3].SSE = _mm_sub_ps(Left.Columns[3].SSE, Right.Columns[3].SSE);
#else
    Result.Elements[0][0] = Left.Elements[0][0] - Right.Elements[0][0];
    Result.Elements[0][1] = Left.Elements[0][1] - Right.Elements[0][1];
    Result.Elements[0][2] = Left.Elements[0][2] - Right.Elements[0][2];
    Result.Elements[0][3] = Left.Elements[0][3] - Right.Elements[0][3];
    Result.Elements[1][0] = Left.Elements[1][0] - Right.Elements[1][0];
    Result.Elements[1][1] = Left.Elements[1][1] - Right.Elements[1][1];
    Result.Elements[1][2] = Left.Elements[1][2] - Right.Elements[1][2];
    Result.Elements[1][3] = Left.Elements[1][3] - Right.Elements[1][3];
    Result.Elements[2][0] = Left.Elements[2][0] - Right.Elements[2][0];
    Result.Elements[2][1] = Left.Elements[2][1] - Right.Elements[2][1];
    Result.Elements[2][2] = Left.Elements[2][2] - Right.Elements[2][2];
    Result.Elements[2][3] = Left.Elements[2][3] - Right.Elements[2][3];
    Result.Elements[3][0] = Left.Elements[3][0] - Right.Elements[3][0];
    Result.Elements[3][1] = Left.Elements[3][1] - Right.Elements[3][1];
    Result.Elements[3][2] = Left.Elements[3][2] - Right.Elements[3][2];
    Result.Elements[3][3] = Left.Elements[3][3] - Right.Elements[3][3];
#endif
 
    return Result;
}

COVERAGE(HMMD_MulM4, 1)
static inline HMMD_Mat4 HMMD_MulM4(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_MulM4);

    HMMD_Mat4 Result;
    Result.Columns[0] = HMMD_LinearCombineV4M4(Right.Columns[0], Left);
    Result.Columns[1] = HMMD_LinearCombineV4M4(Right.Columns[1], Left);
    Result.Columns[2] = HMMD_LinearCombineV4M4(Right.Columns[2], Left);
    Result.Columns[3] = HMMD_LinearCombineV4M4(Right.Columns[3], Left);

    return Result;
}

COVERAGE(HMMD_MulM4F, 1)
static inline HMMD_Mat4 HMMD_MulM4F(HMMD_Mat4 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_MulM4F);

    HMMD_Mat4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.Columns[0].SSE = _mm_mul_ps(Matrix.Columns[0].SSE, SSEScalar);
    Result.Columns[1].SSE = _mm_mul_ps(Matrix.Columns[1].SSE, SSEScalar);
    Result.Columns[2].SSE = _mm_mul_ps(Matrix.Columns[2].SSE, SSEScalar);
    Result.Columns[3].SSE = _mm_mul_ps(Matrix.Columns[3].SSE, SSEScalar);
#else
    Result.Elements[0][0] = Matrix.Elements[0][0] * Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] * Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] * Scalar;
    Result.Elements[0][3] = Matrix.Elements[0][3] * Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] * Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] * Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] * Scalar;
    Result.Elements[1][3] = Matrix.Elements[1][3] * Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] * Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] * Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] * Scalar;
    Result.Elements[2][3] = Matrix.Elements[2][3] * Scalar;
    Result.Elements[3][0] = Matrix.Elements[3][0] * Scalar;
    Result.Elements[3][1] = Matrix.Elements[3][1] * Scalar;
    Result.Elements[3][2] = Matrix.Elements[3][2] * Scalar;
    Result.Elements[3][3] = Matrix.Elements[3][3] * Scalar;
#endif

    return Result;
}

COVERAGE(HMMD_MulM4V4, 1)
static inline HMMD_Vec4 HMMD_MulM4V4(HMMD_Mat4 Matrix, HMMD_Vec4 Vector)
{
    ASSERT_COVERED(HMMD_MulM4V4);
    return HMMD_LinearCombineV4M4(Vector, Matrix);
}

COVERAGE(HMMD_DivM4F, 1)
static inline HMMD_Mat4 HMMD_DivM4F(HMMD_Mat4 Matrix, double Scalar)
{
    ASSERT_COVERED(HMMD_DivM4F);

    HMMD_Mat4 Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEScalar = _mm_set1_ps(Scalar);
    Result.Columns[0].SSE = _mm_div_ps(Matrix.Columns[0].SSE, SSEScalar);
    Result.Columns[1].SSE = _mm_div_ps(Matrix.Columns[1].SSE, SSEScalar);
    Result.Columns[2].SSE = _mm_div_ps(Matrix.Columns[2].SSE, SSEScalar);
    Result.Columns[3].SSE = _mm_div_ps(Matrix.Columns[3].SSE, SSEScalar);
#else
    Result.Elements[0][0] = Matrix.Elements[0][0] / Scalar;
    Result.Elements[0][1] = Matrix.Elements[0][1] / Scalar;
    Result.Elements[0][2] = Matrix.Elements[0][2] / Scalar;
    Result.Elements[0][3] = Matrix.Elements[0][3] / Scalar;
    Result.Elements[1][0] = Matrix.Elements[1][0] / Scalar;
    Result.Elements[1][1] = Matrix.Elements[1][1] / Scalar;
    Result.Elements[1][2] = Matrix.Elements[1][2] / Scalar;
    Result.Elements[1][3] = Matrix.Elements[1][3] / Scalar;
    Result.Elements[2][0] = Matrix.Elements[2][0] / Scalar;
    Result.Elements[2][1] = Matrix.Elements[2][1] / Scalar;
    Result.Elements[2][2] = Matrix.Elements[2][2] / Scalar;
    Result.Elements[2][3] = Matrix.Elements[2][3] / Scalar;
    Result.Elements[3][0] = Matrix.Elements[3][0] / Scalar;
    Result.Elements[3][1] = Matrix.Elements[3][1] / Scalar;
    Result.Elements[3][2] = Matrix.Elements[3][2] / Scalar;
    Result.Elements[3][3] = Matrix.Elements[3][3] / Scalar;
#endif

    return Result;
}

COVERAGE(HMMD_DeterminantM4, 1)
static inline double HMMD_DeterminantM4(HMMD_Mat4 Matrix) 
{
    ASSERT_COVERED(HMMD_DeterminantM4);

    HMMD_Vec3 C01 = HMMD_Cross(Matrix.Columns[0].XYZ, Matrix.Columns[1].XYZ);
    HMMD_Vec3 C23 = HMMD_Cross(Matrix.Columns[2].XYZ, Matrix.Columns[3].XYZ);
    HMMD_Vec3 B10 = HMMD_SubV3(HMMD_MulV3F(Matrix.Columns[0].XYZ, Matrix.Columns[1].W), HMMD_MulV3F(Matrix.Columns[1].XYZ, Matrix.Columns[0].W));
    HMMD_Vec3 B32 = HMMD_SubV3(HMMD_MulV3F(Matrix.Columns[2].XYZ, Matrix.Columns[3].W), HMMD_MulV3F(Matrix.Columns[3].XYZ, Matrix.Columns[2].W));
    
    return HMMD_DotV3(C01, B32) + HMMD_DotV3(C23, B10);
}

COVERAGE(HMMD_InvGeneralM4, 1)
// Returns a general-purpose inverse of an HMMD_Mat4. Note that special-purpose inverses of many transformations
// are available and will be more efficient.
static inline HMMD_Mat4 HMMD_InvGeneralM4(HMMD_Mat4 Matrix) 
{
    ASSERT_COVERED(HMMD_InvGeneralM4);

    HMMD_Vec3 C01 = HMMD_Cross(Matrix.Columns[0].XYZ, Matrix.Columns[1].XYZ);
    HMMD_Vec3 C23 = HMMD_Cross(Matrix.Columns[2].XYZ, Matrix.Columns[3].XYZ);
    HMMD_Vec3 B10 = HMMD_SubV3(HMMD_MulV3F(Matrix.Columns[0].XYZ, Matrix.Columns[1].W), HMMD_MulV3F(Matrix.Columns[1].XYZ, Matrix.Columns[0].W));
    HMMD_Vec3 B32 = HMMD_SubV3(HMMD_MulV3F(Matrix.Columns[2].XYZ, Matrix.Columns[3].W), HMMD_MulV3F(Matrix.Columns[3].XYZ, Matrix.Columns[2].W));
    
    double InvDeterminant = 1.0f / (HMMD_DotV3(C01, B32) + HMMD_DotV3(C23, B10));
    C01 = HMMD_MulV3F(C01, InvDeterminant);
    C23 = HMMD_MulV3F(C23, InvDeterminant);
    B10 = HMMD_MulV3F(B10, InvDeterminant);
    B32 = HMMD_MulV3F(B32, InvDeterminant);

    HMMD_Mat4 Result;
    Result.Columns[0] = HMMD_V4V(HMMD_AddV3(HMMD_Cross(Matrix.Columns[1].XYZ, B32), HMMD_MulV3F(C23, Matrix.Columns[1].W)), -HMMD_DotV3(Matrix.Columns[1].XYZ, C23));
    Result.Columns[1] = HMMD_V4V(HMMD_SubV3(HMMD_Cross(B32, Matrix.Columns[0].XYZ), HMMD_MulV3F(C23, Matrix.Columns[0].W)), +HMMD_DotV3(Matrix.Columns[0].XYZ, C23));
    Result.Columns[2] = HMMD_V4V(HMMD_AddV3(HMMD_Cross(Matrix.Columns[3].XYZ, B10), HMMD_MulV3F(C01, Matrix.Columns[3].W)), -HMMD_DotV3(Matrix.Columns[3].XYZ, C01));
    Result.Columns[3] = HMMD_V4V(HMMD_SubV3(HMMD_Cross(B10, Matrix.Columns[2].XYZ), HMMD_MulV3F(C01, Matrix.Columns[2].W)), +HMMD_DotV3(Matrix.Columns[2].XYZ, C01));
        
    return HMMD_TransposeM4(Result);
}

/*
 * Common graphics transformations
 */

COVERAGE(HMMD_Orthographic_RH_NO, 1)
// Produces a right-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline HMMD_Mat4 HMMD_Orthographic_RH_NO(double Left, double Right, double Bottom, double Top, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Orthographic_RH_NO);

    HMMD_Mat4 Result = {0};

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);
    Result.Elements[3][3] = 1.0f;

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Near + Far) / (Near - Far);

    return Result;
}

COVERAGE(HMMD_Orthographic_RH_ZO, 1)
// Produces a right-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline HMMD_Mat4 HMMD_Orthographic_RH_ZO(double Left, double Right, double Bottom, double Top, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Orthographic_RH_ZO);

    HMMD_Mat4 Result = {0};

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 1.0f / (Near - Far);
    Result.Elements[3][3] = 1.0f;

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Near) / (Near - Far);

    return Result;
}

COVERAGE(HMMD_Orthographic_LH_NO, 1)
// Produces a left-handed orthographic projection matrix with Z ranging from -1 to 1 (the GL convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline HMMD_Mat4 HMMD_Orthographic_LH_NO(double Left, double Right, double Bottom, double Top, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Orthographic_LH_NO);

    HMMD_Mat4 Result = HMMD_Orthographic_RH_NO(Left, Right, Bottom, Top, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    
    return Result;
}

COVERAGE(HMMD_Orthographic_LH_ZO, 1)
// Produces a left-handed orthographic projection matrix with Z ranging from 0 to 1 (the DirectX convention).
// Left, Right, Bottom, and Top specify the coordinates of their respective clipping planes.
// Near and Far specify the distances to the near and far clipping planes.
static inline HMMD_Mat4 HMMD_Orthographic_LH_ZO(double Left, double Right, double Bottom, double Top, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Orthographic_LH_ZO);

    HMMD_Mat4 Result = HMMD_Orthographic_RH_ZO(Left, Right, Bottom, Top, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    
    return Result;
}

COVERAGE(HMMD_InvOrthographic, 1)
// Returns an inverse for the given orthographic projection matrix. Works for all orthographic
// projection matrices, regardless of handedness or NDC convention.
static inline HMMD_Mat4 HMMD_InvOrthographic(HMMD_Mat4 OrthoMatrix)
{
    ASSERT_COVERED(HMMD_InvOrthographic);

    HMMD_Mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / OrthoMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / OrthoMatrix.Elements[1][1];
    Result.Elements[2][2] = 1.0f / OrthoMatrix.Elements[2][2];
    Result.Elements[3][3] = 1.0f;
    
    Result.Elements[3][0] = -OrthoMatrix.Elements[3][0] * Result.Elements[0][0];
    Result.Elements[3][1] = -OrthoMatrix.Elements[3][1] * Result.Elements[1][1];
    Result.Elements[3][2] = -OrthoMatrix.Elements[3][2] * Result.Elements[2][2];

    return Result;
}

COVERAGE(HMMD_Perspective_RH_NO, 1)
static inline HMMD_Mat4 HMMD_Perspective_RH_NO(double FOV, double AspectRatio, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Perspective_RH_NO);

    HMMD_Mat4 Result = {0};

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    double Cotangent = 1.0f / HMMD_TanF(FOV / 2.0f);
    Result.Elements[0][0] = Cotangent / AspectRatio;
    Result.Elements[1][1] = Cotangent;
    Result.Elements[2][3] = -1.0f;

    Result.Elements[2][2] = (Near + Far) / (Near - Far);
    Result.Elements[3][2] = (2.0f * Near * Far) / (Near - Far);
    
    return Result;
}

COVERAGE(HMMD_Perspective_RH_ZO, 1)
static inline HMMD_Mat4 HMMD_Perspective_RH_ZO(double FOV, double AspectRatio, double Near, double Far)
{
    ASSERT_COVERED(HMMD_Perspective_RH_ZO);

    HMMD_Mat4 Result = {0};

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    double Cotangent = 1.0f / HMMD_TanF(FOV / 2.0f);
    Result.Elements[0][0] = Cotangent / AspectRatio;
    Result.Elements[1][1] = Cotangent;
    Result.Elements[2][3] = -1.0f;

    Result.Elements[2][2] = (Far) / (Near - Far);
    Result.Elements[3][2] = (Near * Far) / (Near - Far);

    return Result;
}

COVERAGE(HMMD_Perspective_LH_NO, 1)
static inline HMMD_Mat4 HMMD_Perspective_LH_NO(double FOV, double AspectRatio, double Near, double Far)
{ 
    ASSERT_COVERED(HMMD_Perspective_LH_NO);

    HMMD_Mat4 Result = HMMD_Perspective_RH_NO(FOV, AspectRatio, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    Result.Elements[2][3] = -Result.Elements[2][3];
    
    return Result;
}

COVERAGE(HMMD_Perspective_LH_ZO, 1)
static inline HMMD_Mat4 HMMD_Perspective_LH_ZO(double FOV, double AspectRatio, double Near, double Far)
{ 
    ASSERT_COVERED(HMMD_Perspective_LH_ZO);

    HMMD_Mat4 Result = HMMD_Perspective_RH_ZO(FOV, AspectRatio, Near, Far);
    Result.Elements[2][2] = -Result.Elements[2][2];
    Result.Elements[2][3] = -Result.Elements[2][3];
    
    return Result;
}

COVERAGE(HMMD_InvPerspective_RH, 1)
static inline HMMD_Mat4 HMMD_InvPerspective_RH(HMMD_Mat4 PerspectiveMatrix)
{
    ASSERT_COVERED(HMMD_InvPerspective_RH);

    HMMD_Mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / PerspectiveMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / PerspectiveMatrix.Elements[1][1];
    Result.Elements[2][2] = 0.0f;

    Result.Elements[2][3] = 1.0f / PerspectiveMatrix.Elements[3][2];
    Result.Elements[3][3] = PerspectiveMatrix.Elements[2][2] * Result.Elements[2][3];
    Result.Elements[3][2] = PerspectiveMatrix.Elements[2][3];

    return Result;
}

COVERAGE(HMMD_InvPerspective_LH, 1)
static inline HMMD_Mat4 HMMD_InvPerspective_LH(HMMD_Mat4 PerspectiveMatrix)
{
    ASSERT_COVERED(HMMD_InvPerspective_LH);

    HMMD_Mat4 Result = {0};
    Result.Elements[0][0] = 1.0f / PerspectiveMatrix.Elements[0][0];
    Result.Elements[1][1] = 1.0f / PerspectiveMatrix.Elements[1][1];
    Result.Elements[2][2] = 0.0f;

    Result.Elements[2][3] = 1.0f / PerspectiveMatrix.Elements[3][2];
    Result.Elements[3][3] = PerspectiveMatrix.Elements[2][2] * -Result.Elements[2][3];
    Result.Elements[3][2] = PerspectiveMatrix.Elements[2][3];

    return Result;
}

COVERAGE(HMMD_Translate, 1)
static inline HMMD_Mat4 HMMD_Translate(HMMD_Vec3 Translation)
{
    ASSERT_COVERED(HMMD_Translate);

    HMMD_Mat4 Result = HMMD_M4D(1.0f);
    Result.Elements[3][0] = Translation.X;
    Result.Elements[3][1] = Translation.Y;
    Result.Elements[3][2] = Translation.Z;

    return Result;
}

COVERAGE(HMMD_InvTranslate, 1)
static inline HMMD_Mat4 HMMD_InvTranslate(HMMD_Mat4 TranslationMatrix)
{
    ASSERT_COVERED(HMMD_InvTranslate);

    HMMD_Mat4 Result = TranslationMatrix;
    Result.Elements[3][0] = -Result.Elements[3][0];
    Result.Elements[3][1] = -Result.Elements[3][1];
    Result.Elements[3][2] = -Result.Elements[3][2];

    return Result;
}

COVERAGE(HMMD_Rotate_RH, 1)
static inline HMMD_Mat4 HMMD_Rotate_RH(double Angle, HMMD_Vec3 Axis)
{
    ASSERT_COVERED(HMMD_Rotate_RH);

    HMMD_Mat4 Result = HMMD_M4D(1.0f);

    Axis = HMMD_NormV3(Axis);

    double SinTheta = HMMD_SinF(Angle);
    double CosTheta = HMMD_CosF(Angle);
    double CosValue = 1.0f - CosTheta;

    Result.Elements[0][0] = (Axis.X * Axis.X * CosValue) + CosTheta;
    Result.Elements[0][1] = (Axis.X * Axis.Y * CosValue) + (Axis.Z * SinTheta);
    Result.Elements[0][2] = (Axis.X * Axis.Z * CosValue) - (Axis.Y * SinTheta);

    Result.Elements[1][0] = (Axis.Y * Axis.X * CosValue) - (Axis.Z * SinTheta);
    Result.Elements[1][1] = (Axis.Y * Axis.Y * CosValue) + CosTheta;
    Result.Elements[1][2] = (Axis.Y * Axis.Z * CosValue) + (Axis.X * SinTheta);

    Result.Elements[2][0] = (Axis.Z * Axis.X * CosValue) + (Axis.Y * SinTheta);
    Result.Elements[2][1] = (Axis.Z * Axis.Y * CosValue) - (Axis.X * SinTheta);
    Result.Elements[2][2] = (Axis.Z * Axis.Z * CosValue) + CosTheta;

    return Result;
}

COVERAGE(HMMD_Rotate_LH, 1)
static inline HMMD_Mat4 HMMD_Rotate_LH(double Angle, HMMD_Vec3 Axis)
{
    ASSERT_COVERED(HMMD_Rotate_LH);
    /* NOTE(lcf): Matrix will be inverse/transpose of RH. */
    return HMMD_Rotate_RH(-Angle, Axis);
}

COVERAGE(HMMD_InvRotate, 1)
static inline HMMD_Mat4 HMMD_InvRotate(HMMD_Mat4 RotationMatrix)
{
    ASSERT_COVERED(HMMD_InvRotate);
    return HMMD_TransposeM4(RotationMatrix);
}

COVERAGE(HMMD_Scale, 1)
static inline HMMD_Mat4 HMMD_Scale(HMMD_Vec3 Scale)
{
    ASSERT_COVERED(HMMD_Scale);

    HMMD_Mat4 Result = HMMD_M4D(1.0f);
    Result.Elements[0][0] = Scale.X;
    Result.Elements[1][1] = Scale.Y;
    Result.Elements[2][2] = Scale.Z;

    return Result;
}

COVERAGE(HMMD_InvScale, 1)
static inline HMMD_Mat4 HMMD_InvScale(HMMD_Mat4 ScaleMatrix) 
{
    ASSERT_COVERED(HMMD_InvScale);

    HMMD_Mat4 Result = ScaleMatrix;
    Result.Elements[0][0] = 1.0f / Result.Elements[0][0];
    Result.Elements[1][1] = 1.0f / Result.Elements[1][1];
    Result.Elements[2][2] = 1.0f / Result.Elements[2][2];

    return Result;
}

static inline HMMD_Mat4 _HMMD_LookAt(HMMD_Vec3 F,  HMMD_Vec3 S, HMMD_Vec3 U,  HMMD_Vec3 Eye)
{
    HMMD_Mat4 Result;

    Result.Elements[0][0] = S.X;
    Result.Elements[0][1] = U.X;
    Result.Elements[0][2] = -F.X;
    Result.Elements[0][3] = 0.0f;

    Result.Elements[1][0] = S.Y;
    Result.Elements[1][1] = U.Y;
    Result.Elements[1][2] = -F.Y;
    Result.Elements[1][3] = 0.0f;

    Result.Elements[2][0] = S.Z;
    Result.Elements[2][1] = U.Z;
    Result.Elements[2][2] = -F.Z;
    Result.Elements[2][3] = 0.0f;

    Result.Elements[3][0] = -HMMD_DotV3(S, Eye);
    Result.Elements[3][1] = -HMMD_DotV3(U, Eye);
    Result.Elements[3][2] = HMMD_DotV3(F, Eye);
    Result.Elements[3][3] = 1.0f;

    return Result;
}

COVERAGE(HMMD_LookAt_RH, 1)
static inline HMMD_Mat4 HMMD_LookAt_RH(HMMD_Vec3 Eye, HMMD_Vec3 Center, HMMD_Vec3 Up)
{
    ASSERT_COVERED(HMMD_LookAt_RH);

    HMMD_Vec3 F = HMMD_NormV3(HMMD_SubV3(Center, Eye));
    HMMD_Vec3 S = HMMD_NormV3(HMMD_Cross(F, Up));
    HMMD_Vec3 U = HMMD_Cross(S, F);

    return _HMMD_LookAt(F, S, U, Eye);
}

COVERAGE(HMMD_LookAt_LH, 1)
static inline HMMD_Mat4 HMMD_LookAt_LH(HMMD_Vec3 Eye, HMMD_Vec3 Center, HMMD_Vec3 Up)
{
    ASSERT_COVERED(HMMD_LookAt_LH);

    HMMD_Vec3 F = HMMD_NormV3(HMMD_SubV3(Eye, Center));
    HMMD_Vec3 S = HMMD_NormV3(HMMD_Cross(F, Up));
    HMMD_Vec3 U = HMMD_Cross(S, F);

    return _HMMD_LookAt(F, S, U, Eye);
}

COVERAGE(HMMD_InvLookAt, 1)
static inline HMMD_Mat4 HMMD_InvLookAt(HMMD_Mat4 Matrix)
{
    ASSERT_COVERED(HMMD_InvLookAt);
    HMMD_Mat4 Result;

    HMMD_Mat3 Rotation = {0};
    Rotation.Columns[0] = Matrix.Columns[0].XYZ;
    Rotation.Columns[1] = Matrix.Columns[1].XYZ;
    Rotation.Columns[2] = Matrix.Columns[2].XYZ;
    Rotation = HMMD_TransposeM3(Rotation);

    Result.Columns[0] = HMMD_V4V(Rotation.Columns[0], 0.0f);
    Result.Columns[1] = HMMD_V4V(Rotation.Columns[1], 0.0f);
    Result.Columns[2] = HMMD_V4V(Rotation.Columns[2], 0.0f);
    Result.Columns[3] = HMMD_MulV4F(Matrix.Columns[3], -1.0f);
    Result.Elements[3][0] = -1.0f * Matrix.Elements[3][0] /
        (Rotation.Elements[0][0] + Rotation.Elements[0][1] + Rotation.Elements[0][2]);
    Result.Elements[3][1] = -1.0f * Matrix.Elements[3][1] /
        (Rotation.Elements[1][0] + Rotation.Elements[1][1] + Rotation.Elements[1][2]);
    Result.Elements[3][2] = -1.0f * Matrix.Elements[3][2] /
        (Rotation.Elements[2][0] + Rotation.Elements[2][1] + Rotation.Elements[2][2]);
    Result.Elements[3][3] = 1.0f;

    return Result;
}

/*
 * Quaternion operations
 */

COVERAGE(HMMD_Q, 1)
static inline HMMD_Quat HMMD_Q(double X, double Y, double Z, double W)
{
    ASSERT_COVERED(HMMD_Q);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_setr_ps(X, Y, Z, W);
#else
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
#endif

    return Result;
}

COVERAGE(HMMD_QV4, 1)
static inline HMMD_Quat HMMD_QV4(HMMD_Vec4 Vector)
{
    ASSERT_COVERED(HMMD_QV4);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = Vector.SSE;
#else
    Result.X = Vector.X;
    Result.Y = Vector.Y;
    Result.Z = Vector.Z;
    Result.W = Vector.W;
#endif

    return Result;
}

COVERAGE(HMMD_AddQ, 1)
static inline HMMD_Quat HMMD_AddQ(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_AddQ);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_add_ps(Left.SSE, Right.SSE);
#else

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_SubQ, 1)
static inline HMMD_Quat HMMD_SubQ(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_SubQ);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_sub_ps(Left.SSE, Right.SSE);
#else
    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;
#endif

    return Result;
}

COVERAGE(HMMD_MulQ, 1)
static inline HMMD_Quat HMMD_MulQ(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_MulQ);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
    __m128 SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(0, 1, 2, 3));
    __m128 SSEResultThree = _mm_mul_ps(SSEResultTwo, SSEResultOne);

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(1, 1, 1, 1)) , _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(1, 0, 3, 2));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_xor_ps(_mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultThree = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));

    SSEResultOne = _mm_shuffle_ps(Left.SSE, Left.SSE, _MM_SHUFFLE(3, 3, 3, 3));
    SSEResultTwo = _mm_shuffle_ps(Right.SSE, Right.SSE, _MM_SHUFFLE(3, 2, 1, 0));
    Result.SSE = _mm_add_ps(SSEResultThree, _mm_mul_ps(SSEResultTwo, SSEResultOne));
#else
    Result.X =  Right.Elements[3] * +Left.Elements[0];
    Result.Y =  Right.Elements[2] * -Left.Elements[0];
    Result.Z =  Right.Elements[1] * +Left.Elements[0];
    Result.W =  Right.Elements[0] * -Left.Elements[0];

    Result.X += Right.Elements[2] * +Left.Elements[1];
    Result.Y += Right.Elements[3] * +Left.Elements[1];
    Result.Z += Right.Elements[0] * -Left.Elements[1];
    Result.W += Right.Elements[1] * -Left.Elements[1];
    
    Result.X += Right.Elements[1] * -Left.Elements[2];
    Result.Y += Right.Elements[0] * +Left.Elements[2];
    Result.Z += Right.Elements[3] * +Left.Elements[2];
    Result.W += Right.Elements[2] * -Left.Elements[2];

    Result.X += Right.Elements[0] * +Left.Elements[3];
    Result.Y += Right.Elements[1] * +Left.Elements[3];
    Result.Z += Right.Elements[2] * +Left.Elements[3];
    Result.W += Right.Elements[3] * +Left.Elements[3];
#endif

    return Result;
}

COVERAGE(HMMD_MulQF, 1)
static inline HMMD_Quat HMMD_MulQF(HMMD_Quat Left, double Multiplicative)
{
    ASSERT_COVERED(HMMD_MulQF);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Multiplicative);
    Result.SSE = _mm_mul_ps(Left.SSE, Scalar);
#else
    Result.X = Left.X * Multiplicative;
    Result.Y = Left.Y * Multiplicative;
    Result.Z = Left.Z * Multiplicative;
    Result.W = Left.W * Multiplicative;
#endif

    return Result;
}

COVERAGE(HMMD_DivQF, 1)
static inline HMMD_Quat HMMD_DivQF(HMMD_Quat Left, double Divnd)
{
    ASSERT_COVERED(HMMD_DivQF);

    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Scalar = _mm_set1_ps(Divnd);
    Result.SSE = _mm_div_ps(Left.SSE, Scalar);
#else
    Result.X = Left.X / Divnd;
    Result.Y = Left.Y / Divnd;
    Result.Z = Left.Z / Divnd;
    Result.W = Left.W / Divnd;
#endif

    return Result;
}

COVERAGE(HMMD_DotQ, 1)
static inline double HMMD_DotQ(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_DotQ);

    double Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, Right.SSE);
    __m128 SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    SSEResultTwo = _mm_shuffle_ps(SSEResultOne, SSEResultOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSEResultOne = _mm_add_ps(SSEResultOne, SSEResultTwo);
    _mm_store_ss(&Result, SSEResultOne);
#else
    Result = ((Left.X * Right.X) + (Left.Z * Right.Z)) + ((Left.Y * Right.Y) + (Left.W * Right.W));
#endif

    return Result;
}

COVERAGE(HMMD_InvQ, 1)
static inline HMMD_Quat HMMD_InvQ(HMMD_Quat Left)
{
    ASSERT_COVERED(HMMD_InvQ);
    
    HMMD_Quat Result;
    Result.X = -Left.X;
    Result.Y = -Left.Y;
    Result.Z = -Left.Z;
    Result.W = Left.W;

    return HMMD_DivQF(Result, (HMMD_DotQ(Left, Left)));
}

COVERAGE(HMMD_NormQ, 1)
static inline HMMD_Quat HMMD_NormQ(HMMD_Quat Quat)
{
    ASSERT_COVERED(HMMD_NormQ);

    /* NOTE(lcf): Take advantage of SSE implementation in HMMD_NormV4 */
    HMMD_Vec4 Vec = {Quat.X, Quat.Y, Quat.Z, Quat.W};
    Vec = HMMD_NormV4(Vec);
    HMMD_Quat Result = {Vec.X, Vec.Y, Vec.Z, Vec.W};

    return Result;
}

static inline HMMD_Quat _HMMD_MixQ(HMMD_Quat Left, double MixLeft, HMMD_Quat Right, double MixRight) {
    HMMD_Quat Result;

#ifdef HANDMADE_MATH__USE_SSE
    __m128 ScalarLeft = _mm_set1_ps(MixLeft);
    __m128 ScalarRight = _mm_set1_ps(MixRight);
    __m128 SSEResultOne = _mm_mul_ps(Left.SSE, ScalarLeft);
    __m128 SSEResultTwo = _mm_mul_ps(Right.SSE, ScalarRight);
    Result.SSE = _mm_add_ps(SSEResultOne, SSEResultTwo);
#else
    Result.X = Left.X*MixLeft + Right.X*MixRight;
    Result.Y = Left.Y*MixLeft + Right.Y*MixRight;
    Result.Z = Left.Z*MixLeft + Right.Z*MixRight;
    Result.W = Left.W*MixLeft + Right.W*MixRight;
#endif

    return Result;
}

COVERAGE(HMMD_NLerp, 1)
static inline HMMD_Quat HMMD_NLerp(HMMD_Quat Left, double Time, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_NLerp);

    HMMD_Quat Result = _HMMD_MixQ(Left, 1.0f-Time, Right, Time);
    Result = HMMD_NormQ(Result);

    return Result;
}

COVERAGE(HMMD_SLerp, 1)
static inline HMMD_Quat HMMD_SLerp(HMMD_Quat Left, double Time, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_SLerp);

    HMMD_Quat Result;

    double Cos_Theta = HMMD_DotQ(Left, Right);

    if (Cos_Theta < 0.0f) { /* NOTE(lcf): Take shortest path on Hyper-sphere */
        Cos_Theta = -Cos_Theta;
        Right = HMMD_Q(-Right.X, -Right.Y, -Right.Z, -Right.W);
    }
    
    /* NOTE(lcf): Use Normalized Linear interpolation when vectors are roughly not L.I. */
    if (Cos_Theta > 0.9995f) {
        Result = HMMD_NLerp(Left, Time, Right);
    } else {
        double Angle = HMMD_ACosF(Cos_Theta);
        double MixLeft = HMMD_SinF((1.0f - Time) * Angle);
        double MixRight = HMMD_SinF(Time * Angle);

        Result = _HMMD_MixQ(Left, MixLeft, Right, MixRight);
        Result = HMMD_NormQ(Result);
    }
    
    return Result;
}

COVERAGE(HMMD_QToM4, 1)
static inline HMMD_Mat4 HMMD_QToM4(HMMD_Quat Left)
{
    ASSERT_COVERED(HMMD_QToM4);

    HMMD_Mat4 Result;

    HMMD_Quat NormalizedQ = HMMD_NormQ(Left);

    double XX, YY, ZZ,
          XY, XZ, YZ,
          WX, WY, WZ;

    XX = NormalizedQ.X * NormalizedQ.X;
    YY = NormalizedQ.Y * NormalizedQ.Y;
    ZZ = NormalizedQ.Z * NormalizedQ.Z;
    XY = NormalizedQ.X * NormalizedQ.Y;
    XZ = NormalizedQ.X * NormalizedQ.Z;
    YZ = NormalizedQ.Y * NormalizedQ.Z;
    WX = NormalizedQ.W * NormalizedQ.X;
    WY = NormalizedQ.W * NormalizedQ.Y;
    WZ = NormalizedQ.W * NormalizedQ.Z;

    Result.Elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.Elements[0][1] = 2.0f * (XY + WZ);
    Result.Elements[0][2] = 2.0f * (XZ - WY);
    Result.Elements[0][3] = 0.0f;

    Result.Elements[1][0] = 2.0f * (XY - WZ);
    Result.Elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.Elements[1][2] = 2.0f * (YZ + WX);
    Result.Elements[1][3] = 0.0f;

    Result.Elements[2][0] = 2.0f * (XZ + WY);
    Result.Elements[2][1] = 2.0f * (YZ - WX);
    Result.Elements[2][2] = 1.0f - 2.0f * (XX + YY);
    Result.Elements[2][3] = 0.0f;

    Result.Elements[3][0] = 0.0f;
    Result.Elements[3][1] = 0.0f;
    Result.Elements[3][2] = 0.0f;
    Result.Elements[3][3] = 1.0f;

    return Result;
}

// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be *post*-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices *appear* to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like M.Elements[1][2].
//
// Don't be confused! Or if you must be confused, at least trust this
// comment. :)
COVERAGE(HMMD_M4ToQ_RH, 4)
static inline HMMD_Quat HMMD_M4ToQ_RH(HMMD_Mat4 M)
{
    double T;
    HMMD_Quat Q;

    if (M.Elements[2][2] < 0.0f) {
        if (M.Elements[0][0] > M.Elements[1][1]) {
            ASSERT_COVERED(HMMD_M4ToQ_RH);

            T = 1 + M.Elements[0][0] - M.Elements[1][1] - M.Elements[2][2];
            Q = HMMD_Q(
                T,
                M.Elements[0][1] + M.Elements[1][0],
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] - M.Elements[2][1]
            );
        } else {
            ASSERT_COVERED(HMMD_M4ToQ_RH);

            T = 1 - M.Elements[0][0] + M.Elements[1][1] - M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[0][1] + M.Elements[1][0],
                T,
                M.Elements[1][2] + M.Elements[2][1],
                M.Elements[2][0] - M.Elements[0][2]
            );
        }
    } else {
        if (M.Elements[0][0] < -M.Elements[1][1]) {
            ASSERT_COVERED(HMMD_M4ToQ_RH);

            T = 1 - M.Elements[0][0] - M.Elements[1][1] + M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] + M.Elements[2][1],
                T,
                M.Elements[0][1] - M.Elements[1][0]
            );
        } else {
            ASSERT_COVERED(HMMD_M4ToQ_RH);

            T = 1 + M.Elements[0][0] + M.Elements[1][1] + M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[1][2] - M.Elements[2][1],
                M.Elements[2][0] - M.Elements[0][2],
                M.Elements[0][1] - M.Elements[1][0],
                T
            );
        }
    }

    Q = HMMD_MulQF(Q, 0.5f / HMMD_SqrtF(T));

    return Q;
}

COVERAGE(HMMD_M4ToQ_LH, 4)
static inline HMMD_Quat HMMD_M4ToQ_LH(HMMD_Mat4 M)
{
    double T;
    HMMD_Quat Q;

    if (M.Elements[2][2] < 0.0f) {
        if (M.Elements[0][0] > M.Elements[1][1]) {
            ASSERT_COVERED(HMMD_M4ToQ_LH);

            T = 1 + M.Elements[0][0] - M.Elements[1][1] - M.Elements[2][2];
            Q = HMMD_Q(
                T,
                M.Elements[0][1] + M.Elements[1][0],
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[2][1] - M.Elements[1][2]
            );
        } else {
            ASSERT_COVERED(HMMD_M4ToQ_LH);

            T = 1 - M.Elements[0][0] + M.Elements[1][1] - M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[0][1] + M.Elements[1][0],
                T,
                M.Elements[1][2] + M.Elements[2][1],
                M.Elements[0][2] - M.Elements[2][0]
            );
        }
    } else {
        if (M.Elements[0][0] < -M.Elements[1][1]) {
            ASSERT_COVERED(HMMD_M4ToQ_LH);

            T = 1 - M.Elements[0][0] - M.Elements[1][1] + M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[2][0] + M.Elements[0][2],
                M.Elements[1][2] + M.Elements[2][1],
                T,
                M.Elements[1][0] - M.Elements[0][1]
            );
        } else {
            ASSERT_COVERED(HMMD_M4ToQ_LH);

            T = 1 + M.Elements[0][0] + M.Elements[1][1] + M.Elements[2][2];
            Q = HMMD_Q(
                M.Elements[2][1] - M.Elements[1][2],
                M.Elements[0][2] - M.Elements[2][0],
                M.Elements[1][0] - M.Elements[0][2],
                T
            );
        }
    }

    Q = HMMD_MulQF(Q, 0.5f / HMMD_SqrtF(T));

    return Q;
}


COVERAGE(HMMD_QFromAxisAngle_RH, 1)
static inline HMMD_Quat HMMD_QFromAxisAngle_RH(HMMD_Vec3 Axis, double AngleOfRotation)
{
    ASSERT_COVERED(HMMD_QFromAxisAngle_RH);

    HMMD_Quat Result;

    HMMD_Vec3 AxisNormalized = HMMD_NormV3(Axis);
    double SineOfRotation = HMMD_SinF(AngleOfRotation / 2.0f);

    Result.XYZ = HMMD_MulV3F(AxisNormalized, SineOfRotation);
    Result.W = HMMD_CosF(AngleOfRotation / 2.0f);

    return Result;
}

COVERAGE(HMMD_QFromAxisAngle_LH, 1)
static inline HMMD_Quat HMMD_QFromAxisAngle_LH(HMMD_Vec3 Axis, double AngleOfRotation)
{
    ASSERT_COVERED(HMMD_QFromAxisAngle_LH);

    return HMMD_QFromAxisAngle_RH(Axis, -AngleOfRotation);
}


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

COVERAGE(HMMD_LenV2CPP, 1)
static inline double HMMD_Len(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_LenV2CPP);
    return HMMD_LenV2(A);
}

COVERAGE(HMMD_LenV3CPP, 1)
static inline double HMMD_Len(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_LenV3CPP);
    return HMMD_LenV3(A);
}

COVERAGE(HMMD_LenV4CPP, 1)
static inline double HMMD_Len(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_LenV4CPP);
    return HMMD_LenV4(A);
}

COVERAGE(HMMD_LenSqrV2CPP, 1)
static inline double HMMD_LenSqr(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_LenSqrV2CPP);
    return HMMD_LenSqrV2(A);
}

COVERAGE(HMMD_LenSqrV3CPP, 1)
static inline double HMMD_LenSqr(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_LenSqrV3CPP);
    return HMMD_LenSqrV3(A);
}

COVERAGE(HMMD_LenSqrV4CPP, 1)
static inline double HMMD_LenSqr(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_LenSqrV4CPP);
    return HMMD_LenSqrV4(A);
}

COVERAGE(HMMD_NormV2CPP, 1)
static inline HMMD_Vec2 HMMD_Norm(HMMD_Vec2 A)
{
    ASSERT_COVERED(HMMD_NormV2CPP);
    return HMMD_NormV2(A);
}

COVERAGE(HMMD_NormV3CPP, 1)
static inline HMMD_Vec3 HMMD_Norm(HMMD_Vec3 A)
{
    ASSERT_COVERED(HMMD_NormV3CPP);
    return HMMD_NormV3(A);
}

COVERAGE(HMMD_NormV4CPP, 1)
static inline HMMD_Vec4 HMMD_Norm(HMMD_Vec4 A)
{
    ASSERT_COVERED(HMMD_NormV4CPP);
    return HMMD_NormV4(A);
}

COVERAGE(HMMD_NormQCPP, 1)
static inline HMMD_Quat HMMD_Norm(HMMD_Quat A)
{
    ASSERT_COVERED(HMMD_NormQCPP);
    return HMMD_NormQ(A);
}

COVERAGE(HMMD_DotV2CPP, 1)
static inline double HMMD_Dot(HMMD_Vec2 Left, HMMD_Vec2 VecTwo)
{
    ASSERT_COVERED(HMMD_DotV2CPP);
    return HMMD_DotV2(Left, VecTwo);
}

COVERAGE(HMMD_DotV3CPP, 1)
static inline double HMMD_Dot(HMMD_Vec3 Left, HMMD_Vec3 VecTwo)
{
    ASSERT_COVERED(HMMD_DotV3CPP);
    return HMMD_DotV3(Left, VecTwo);
}

COVERAGE(HMMD_DotV4CPP, 1)
static inline double HMMD_Dot(HMMD_Vec4 Left, HMMD_Vec4 VecTwo)
{
    ASSERT_COVERED(HMMD_DotV4CPP);
    return HMMD_DotV4(Left, VecTwo);
}
 
COVERAGE(HMMD_LerpV2CPP, 1)
static inline HMMD_Vec2 HMMD_Lerp(HMMD_Vec2 Left, double Time, HMMD_Vec2 Right) 
{
    ASSERT_COVERED(HMMD_LerpV2CPP);
    return HMMD_LerpV2(Left, Time, Right);
}

COVERAGE(HMMD_LerpV3CPP, 1)
static inline HMMD_Vec3 HMMD_Lerp(HMMD_Vec3 Left, double Time, HMMD_Vec3 Right) 
{
    ASSERT_COVERED(HMMD_LerpV3CPP);
    return HMMD_LerpV3(Left, Time, Right);
}

COVERAGE(HMMD_LerpV4CPP, 1)
static inline HMMD_Vec4 HMMD_Lerp(HMMD_Vec4 Left, double Time, HMMD_Vec4 Right) 
{
    ASSERT_COVERED(HMMD_LerpV4CPP);
    return HMMD_LerpV4(Left, Time, Right);
}

COVERAGE(HMMD_TransposeM2CPP, 1)
static inline HMMD_Mat2 HMMD_Transpose(HMMD_Mat2 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM2CPP);
    return HMMD_TransposeM2(Matrix);
}

COVERAGE(HMMD_TransposeM3CPP, 1)
static inline HMMD_Mat3 HMMD_Transpose(HMMD_Mat3 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM3CPP);
    return HMMD_TransposeM3(Matrix);
}

COVERAGE(HMMD_TransposeM4CPP, 1)
static inline HMMD_Mat4 HMMD_Transpose(HMMD_Mat4 Matrix)
{
    ASSERT_COVERED(HMMD_TransposeM4CPP);
    return HMMD_TransposeM4(Matrix);
}

COVERAGE(HMMD_DeterminantM2CPP, 1)
static inline double HMMD_Determinant(HMMD_Mat2 Matrix)
{
    ASSERT_COVERED(HMMD_DeterminantM2CPP);
    return HMMD_DeterminantM2(Matrix);
}

COVERAGE(HMMD_DeterminantM3CPP, 1)
static inline double HMMD_Determinant(HMMD_Mat3 Matrix)
{
    ASSERT_COVERED(HMMD_DeterminantM3CPP);
    return HMMD_DeterminantM3(Matrix);
}

COVERAGE(HMMD_DeterminantM4CPP, 1)
static inline double HMMD_Determinant(HMMD_Mat4 Matrix)
{
    ASSERT_COVERED(HMMD_DeterminantM4CPP);
    return HMMD_DeterminantM4(Matrix);
}

COVERAGE(HMMD_InvGeneralM2CPP, 1)
static inline HMMD_Mat2 HMMD_InvGeneral(HMMD_Mat2 Matrix)
{
    ASSERT_COVERED(HMMD_InvGeneralM2CPP);
    return HMMD_InvGeneralM2(Matrix);
}

COVERAGE(HMMD_InvGeneralM3CPP, 1)
static inline HMMD_Mat3 HMMD_InvGeneral(HMMD_Mat3 Matrix)
{
    ASSERT_COVERED(HMMD_InvGeneralM3CPP);
    return HMMD_InvGeneralM3(Matrix);
}

COVERAGE(HMMD_InvGeneralM4CPP, 1)
static inline HMMD_Mat4 HMMD_InvGeneral(HMMD_Mat4 Matrix)
{
    ASSERT_COVERED(HMMD_InvGeneralM4CPP);
    return HMMD_InvGeneralM4(Matrix);
}

COVERAGE(HMMD_DotQCPP, 1)
static inline double HMMD_Dot(HMMD_Quat QuatOne, HMMD_Quat QuatTwo)
{
    ASSERT_COVERED(HMMD_DotQCPP);
    return HMMD_DotQ(QuatOne, QuatTwo);
}

COVERAGE(HMMD_AddV2CPP, 1)
static inline HMMD_Vec2 HMMD_Add(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_AddV2CPP);
    return HMMD_AddV2(Left, Right);
}

COVERAGE(HMMD_AddV3CPP, 1)
static inline HMMD_Vec3 HMMD_Add(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_AddV3CPP);
    return HMMD_AddV3(Left, Right);
}

COVERAGE(HMMD_AddV4CPP, 1)
static inline HMMD_Vec4 HMMD_Add(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_AddV4CPP);
    return HMMD_AddV4(Left, Right);
}

COVERAGE(HMMD_AddM2CPP, 1)
static inline HMMD_Mat2 HMMD_Add(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_AddM2CPP);
    return HMMD_AddM2(Left, Right);
}

COVERAGE(HMMD_AddM3CPP, 1)
static inline HMMD_Mat3 HMMD_Add(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_AddM3CPP);
    return HMMD_AddM3(Left, Right);
}

COVERAGE(HMMD_AddM4CPP, 1)
static inline HMMD_Mat4 HMMD_Add(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_AddM4CPP);
    return HMMD_AddM4(Left, Right);
}

COVERAGE(HMMD_AddQCPP, 1)
static inline HMMD_Quat HMMD_Add(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_AddQCPP);
    return HMMD_AddQ(Left, Right);
}

COVERAGE(HMMD_SubV2CPP, 1)
static inline HMMD_Vec2 HMMD_Sub(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_SubV2CPP);
    return HMMD_SubV2(Left, Right);
}

COVERAGE(HMMD_SubV3CPP, 1)
static inline HMMD_Vec3 HMMD_Sub(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_SubV3CPP);
    return HMMD_SubV3(Left, Right);
}

COVERAGE(HMMD_SubV4CPP, 1)
static inline HMMD_Vec4 HMMD_Sub(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_SubV4CPP);
    return HMMD_SubV4(Left, Right);
}

COVERAGE(HMMD_SubM2CPP, 1)
static inline HMMD_Mat2 HMMD_Sub(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_SubM2CPP);
    return HMMD_SubM2(Left, Right);
}

COVERAGE(HMMD_SubM3CPP, 1)
static inline HMMD_Mat3 HMMD_Sub(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_SubM3CPP);
    return HMMD_SubM3(Left, Right);
}

COVERAGE(HMMD_SubM4CPP, 1)
static inline HMMD_Mat4 HMMD_Sub(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_SubM4CPP);
    return HMMD_SubM4(Left, Right);
}

COVERAGE(HMMD_SubQCPP, 1)
static inline HMMD_Quat HMMD_Sub(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_SubQCPP);
    return HMMD_SubQ(Left, Right);
}

COVERAGE(HMMD_MulV2CPP, 1)
static inline HMMD_Vec2 HMMD_Mul(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_MulV2CPP);
    return HMMD_MulV2(Left, Right);
}

COVERAGE(HMMD_MulV2FCPP, 1)
static inline HMMD_Vec2 HMMD_Mul(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV2FCPP);
    return HMMD_MulV2F(Left, Right);
}

COVERAGE(HMMD_MulV3CPP, 1)
static inline HMMD_Vec3 HMMD_Mul(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_MulV3CPP);
    return HMMD_MulV3(Left, Right);
}

COVERAGE(HMMD_MulV3FCPP, 1)
static inline HMMD_Vec3 HMMD_Mul(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV3FCPP);
    return HMMD_MulV3F(Left, Right);
}

COVERAGE(HMMD_MulV4CPP, 1)
static inline HMMD_Vec4 HMMD_Mul(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_MulV4CPP);
    return HMMD_MulV4(Left, Right);
}

COVERAGE(HMMD_MulV4FCPP, 1)
static inline HMMD_Vec4 HMMD_Mul(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV4FCPP);
    return HMMD_MulV4F(Left, Right);
}

COVERAGE(HMMD_MulM2CPP, 1)
static inline HMMD_Mat2 HMMD_Mul(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_MulM2CPP);
    return HMMD_MulM2(Left, Right);
}

COVERAGE(HMMD_MulM3CPP, 1)
static inline HMMD_Mat3 HMMD_Mul(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_MulM3CPP);
    return HMMD_MulM3(Left, Right);
}

COVERAGE(HMMD_MulM4CPP, 1)
static inline HMMD_Mat4 HMMD_Mul(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_MulM4CPP);
    return HMMD_MulM4(Left, Right);
}

COVERAGE(HMMD_MulM2FCPP, 1)
static inline HMMD_Mat2 HMMD_Mul(HMMD_Mat2 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM2FCPP);
    return HMMD_MulM2F(Left, Right);
}

COVERAGE(HMMD_MulM3FCPP, 1)
static inline HMMD_Mat3 HMMD_Mul(HMMD_Mat3 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM3FCPP);
    return HMMD_MulM3F(Left, Right);
}

COVERAGE(HMMD_MulM4FCPP, 1)
static inline HMMD_Mat4 HMMD_Mul(HMMD_Mat4 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM4FCPP);
    return HMMD_MulM4F(Left, Right);
}

COVERAGE(HMMD_MulM2V2CPP, 1)
static inline HMMD_Vec2 HMMD_Mul(HMMD_Mat2 Matrix, HMMD_Vec2 Vector)
{
    ASSERT_COVERED(HMMD_MulM2V2CPP);
    return HMMD_MulM2V2(Matrix, Vector);
}

COVERAGE(HMMD_MulM3V3CPP, 1)
static inline HMMD_Vec3 HMMD_Mul(HMMD_Mat3 Matrix, HMMD_Vec3 Vector)
{
    ASSERT_COVERED(HMMD_MulM3V3CPP);
    return HMMD_MulM3V3(Matrix, Vector);
}

COVERAGE(HMMD_MulM4V4CPP, 1)
static inline HMMD_Vec4 HMMD_Mul(HMMD_Mat4 Matrix, HMMD_Vec4 Vector)
{
    ASSERT_COVERED(HMMD_MulM4V4CPP);
    return HMMD_MulM4V4(Matrix, Vector);
}

COVERAGE(HMMD_MulQCPP, 1)
static inline HMMD_Quat HMMD_Mul(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_MulQCPP);
    return HMMD_MulQ(Left, Right);
}

COVERAGE(HMMD_MulQFCPP, 1)
static inline HMMD_Quat HMMD_Mul(HMMD_Quat Left, double Right)
{
    ASSERT_COVERED(HMMD_MulQFCPP);
    return HMMD_MulQF(Left, Right);
}

COVERAGE(HMMD_DivV2CPP, 1)
static inline HMMD_Vec2 HMMD_Div(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_DivV2CPP);
    return HMMD_DivV2(Left, Right);
}

COVERAGE(HMMD_DivV2FCPP, 1)
static inline HMMD_Vec2 HMMD_Div(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV2FCPP);
    return HMMD_DivV2F(Left, Right);
}

COVERAGE(HMMD_DivV3CPP, 1)
static inline HMMD_Vec3 HMMD_Div(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_DivV3CPP);
    return HMMD_DivV3(Left, Right);
}

COVERAGE(HMMD_DivV3FCPP, 1)
static inline HMMD_Vec3 HMMD_Div(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV3FCPP);
    return HMMD_DivV3F(Left, Right);
}

COVERAGE(HMMD_DivV4CPP, 1)
static inline HMMD_Vec4 HMMD_Div(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_DivV4CPP);
    return HMMD_DivV4(Left, Right);
}

COVERAGE(HMMD_DivV4FCPP, 1)
static inline HMMD_Vec4 HMMD_Div(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV4FCPP);
    return HMMD_DivV4F(Left, Right);
}

COVERAGE(HMMD_DivM2FCPP, 1)
static inline HMMD_Mat2 HMMD_Div(HMMD_Mat2 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM2FCPP);
    return HMMD_DivM2F(Left, Right);
}

COVERAGE(HMMD_DivM3FCPP, 1)
static inline HMMD_Mat3 HMMD_Div(HMMD_Mat3 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM3FCPP);
    return HMMD_DivM3F(Left, Right);
}

COVERAGE(HMMD_DivM4FCPP, 1)
static inline HMMD_Mat4 HMMD_Div(HMMD_Mat4 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM4FCPP);
    return HMMD_DivM4F(Left, Right);
}

COVERAGE(HMMD_DivQFCPP, 1)
static inline HMMD_Quat HMMD_Div(HMMD_Quat Left, double Right)
{
    ASSERT_COVERED(HMMD_DivQFCPP);
    return HMMD_DivQF(Left, Right);
}

COVERAGE(HMMD_EqV2CPP, 1)
static inline HMMD_Bool HMMD_Eq(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_EqV2CPP);
    return HMMD_EqV2(Left, Right);
}

COVERAGE(HMMD_EqV3CPP, 1)
static inline HMMD_Bool HMMD_Eq(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_EqV3CPP);
    return HMMD_EqV3(Left, Right);
}

COVERAGE(HMMD_EqV4CPP, 1)
static inline HMMD_Bool HMMD_Eq(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_EqV4CPP);
    return HMMD_EqV4(Left, Right);
}

COVERAGE(HMMD_AddV2Op, 1)
static inline HMMD_Vec2 operator+(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_AddV2Op);
    return HMMD_AddV2(Left, Right);
}

COVERAGE(HMMD_AddV3Op, 1)
static inline HMMD_Vec3 operator+(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_AddV3Op);
    return HMMD_AddV3(Left, Right);
}

COVERAGE(HMMD_AddV4Op, 1)
static inline HMMD_Vec4 operator+(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_AddV4Op);
    return HMMD_AddV4(Left, Right);
}

COVERAGE(HMMD_AddM2Op, 1)
static inline HMMD_Mat2 operator+(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_AddM2Op);
    return HMMD_AddM2(Left, Right);
}

COVERAGE(HMMD_AddM3Op, 1)
static inline HMMD_Mat3 operator+(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_AddM3Op);
    return HMMD_AddM3(Left, Right);
}

COVERAGE(HMMD_AddM4Op, 1)
static inline HMMD_Mat4 operator+(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_AddM4Op);
    return HMMD_AddM4(Left, Right);
}

COVERAGE(HMMD_AddQOp, 1)
static inline HMMD_Quat operator+(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_AddQOp);
    return HMMD_AddQ(Left, Right);
}

COVERAGE(HMMD_SubV2Op, 1)
static inline HMMD_Vec2 operator-(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_SubV2Op);
    return HMMD_SubV2(Left, Right);
}

COVERAGE(HMMD_SubV3Op, 1)
static inline HMMD_Vec3 operator-(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_SubV3Op);
    return HMMD_SubV3(Left, Right);
}

COVERAGE(HMMD_SubV4Op, 1)
static inline HMMD_Vec4 operator-(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_SubV4Op);
    return HMMD_SubV4(Left, Right);
}

COVERAGE(HMMD_SubM2Op, 1)
static inline HMMD_Mat2 operator-(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_SubM2Op);
    return HMMD_SubM2(Left, Right);
}

COVERAGE(HMMD_SubM3Op, 1)
static inline HMMD_Mat3 operator-(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_SubM3Op);
    return HMMD_SubM3(Left, Right);
}

COVERAGE(HMMD_SubM4Op, 1)
static inline HMMD_Mat4 operator-(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_SubM4Op);
    return HMMD_SubM4(Left, Right);
}

COVERAGE(HMMD_SubQOp, 1)
static inline HMMD_Quat operator-(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_SubQOp);
    return HMMD_SubQ(Left, Right);
}

COVERAGE(HMMD_MulV2Op, 1)
static inline HMMD_Vec2 operator*(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_MulV2Op);
    return HMMD_MulV2(Left, Right);
}

COVERAGE(HMMD_MulV3Op, 1)
static inline HMMD_Vec3 operator*(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_MulV3Op);
    return HMMD_MulV3(Left, Right);
}

COVERAGE(HMMD_MulV4Op, 1)
static inline HMMD_Vec4 operator*(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_MulV4Op);
    return HMMD_MulV4(Left, Right);
}

COVERAGE(HMMD_MulM2Op, 1)
static inline HMMD_Mat2 operator*(HMMD_Mat2 Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_MulM2Op);
    return HMMD_MulM2(Left, Right);
}

COVERAGE(HMMD_MulM3Op, 1)
static inline HMMD_Mat3 operator*(HMMD_Mat3 Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_MulM3Op);
    return HMMD_MulM3(Left, Right);
}

COVERAGE(HMMD_MulM4Op, 1)
static inline HMMD_Mat4 operator*(HMMD_Mat4 Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_MulM4Op);
    return HMMD_MulM4(Left, Right);
}

COVERAGE(HMMD_MulQOp, 1)
static inline HMMD_Quat operator*(HMMD_Quat Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_MulQOp);
    return HMMD_MulQ(Left, Right);
}

COVERAGE(HMMD_MulV2FOp, 1)
static inline HMMD_Vec2 operator*(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV2FOp);
    return HMMD_MulV2F(Left, Right);
}

COVERAGE(HMMD_MulV3FOp, 1)
static inline HMMD_Vec3 operator*(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV3FOp);
    return HMMD_MulV3F(Left, Right);
}

COVERAGE(HMMD_MulV4FOp, 1)
static inline HMMD_Vec4 operator*(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV4FOp);
    return HMMD_MulV4F(Left, Right);
}

COVERAGE(HMMD_MulM2FOp, 1)
static inline HMMD_Mat2 operator*(HMMD_Mat2 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM2FOp);
    return HMMD_MulM2F(Left, Right);
}

COVERAGE(HMMD_MulM3FOp, 1)
static inline HMMD_Mat3 operator*(HMMD_Mat3 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM3FOp);
    return HMMD_MulM3F(Left, Right);
}

COVERAGE(HMMD_MulM4FOp, 1)
static inline HMMD_Mat4 operator*(HMMD_Mat4 Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM4FOp);
    return HMMD_MulM4F(Left, Right);
}

COVERAGE(HMMD_MulQFOp, 1)
static inline HMMD_Quat operator*(HMMD_Quat Left, double Right)
{
    ASSERT_COVERED(HMMD_MulQFOp);
    return HMMD_MulQF(Left, Right);
}

COVERAGE(HMMD_MulV2FOpLeft, 1)
static inline HMMD_Vec2 operator*(double Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_MulV2FOpLeft);
    return HMMD_MulV2F(Right, Left);
}

COVERAGE(HMMD_MulV3FOpLeft, 1)
static inline HMMD_Vec3 operator*(double Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_MulV3FOpLeft);
    return HMMD_MulV3F(Right, Left);
}

COVERAGE(HMMD_MulV4FOpLeft, 1)
static inline HMMD_Vec4 operator*(double Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_MulV4FOpLeft);
    return HMMD_MulV4F(Right, Left);
}

COVERAGE(HMMD_MulM2FOpLeft, 1)
static inline HMMD_Mat2 operator*(double Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_MulM2FOpLeft);
    return HMMD_MulM2F(Right, Left);
}

COVERAGE(HMMD_MulM3FOpLeft, 1)
static inline HMMD_Mat3 operator*(double Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_MulM3FOpLeft);
    return HMMD_MulM3F(Right, Left);
}

COVERAGE(HMMD_MulM4FOpLeft, 1)
static inline HMMD_Mat4 operator*(double Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_MulM4FOpLeft);
    return HMMD_MulM4F(Right, Left);
}

COVERAGE(HMMD_MulQFOpLeft, 1)
static inline HMMD_Quat operator*(double Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_MulQFOpLeft);
    return HMMD_MulQF(Right, Left);
}

COVERAGE(HMMD_MulM2V2Op, 1)
static inline HMMD_Vec2 operator*(HMMD_Mat2 Matrix, HMMD_Vec2 Vector)
{
    ASSERT_COVERED(HMMD_MulM2V2Op);
    return HMMD_MulM2V2(Matrix, Vector);
}

COVERAGE(HMMD_MulM3V3Op, 1)
static inline HMMD_Vec3 operator*(HMMD_Mat3 Matrix, HMMD_Vec3 Vector)
{
    ASSERT_COVERED(HMMD_MulM3V3Op);
    return HMMD_MulM3V3(Matrix, Vector);
}

COVERAGE(HMMD_MulM4V4Op, 1)
static inline HMMD_Vec4 operator*(HMMD_Mat4 Matrix, HMMD_Vec4 Vector)
{
    ASSERT_COVERED(HMMD_MulM4V4Op);
    return HMMD_MulM4V4(Matrix, Vector);
}

COVERAGE(HMMD_DivV2Op, 1)
static inline HMMD_Vec2 operator/(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_DivV2Op);
    return HMMD_DivV2(Left, Right);
}

COVERAGE(HMMD_DivV3Op, 1)
static inline HMMD_Vec3 operator/(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_DivV3Op);
    return HMMD_DivV3(Left, Right);
}

COVERAGE(HMMD_DivV4Op, 1)
static inline HMMD_Vec4 operator/(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_DivV4Op);
    return HMMD_DivV4(Left, Right);
}

COVERAGE(HMMD_DivV2FOp, 1)
static inline HMMD_Vec2 operator/(HMMD_Vec2 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV2FOp);
    return HMMD_DivV2F(Left, Right);
}

COVERAGE(HMMD_DivV3FOp, 1)
static inline HMMD_Vec3 operator/(HMMD_Vec3 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV3FOp);
    return HMMD_DivV3F(Left, Right);
}

COVERAGE(HMMD_DivV4FOp, 1)
static inline HMMD_Vec4 operator/(HMMD_Vec4 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV4FOp);
    return HMMD_DivV4F(Left, Right);
}

COVERAGE(HMMD_DivM4FOp, 1)
static inline HMMD_Mat4 operator/(HMMD_Mat4 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM4FOp);
    return HMMD_DivM4F(Left, Right);
}

COVERAGE(HMMD_DivM3FOp, 1)
static inline HMMD_Mat3 operator/(HMMD_Mat3 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM3FOp);
    return HMMD_DivM3F(Left, Right);
}

COVERAGE(HMMD_DivM2FOp, 1)
static inline HMMD_Mat2 operator/(HMMD_Mat2 Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM2FOp);
    return HMMD_DivM2F(Left, Right);
}

COVERAGE(HMMD_DivQFOp, 1)
static inline HMMD_Quat operator/(HMMD_Quat Left, double Right)
{
    ASSERT_COVERED(HMMD_DivQFOp);
    return HMMD_DivQF(Left, Right);
}

COVERAGE(HMMD_AddV2Assign, 1)
static inline HMMD_Vec2 &operator+=(HMMD_Vec2 &Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_AddV2Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddV3Assign, 1)
static inline HMMD_Vec3 &operator+=(HMMD_Vec3 &Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_AddV3Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddV4Assign, 1)
static inline HMMD_Vec4 &operator+=(HMMD_Vec4 &Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_AddV4Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddM2Assign, 1)
static inline HMMD_Mat2 &operator+=(HMMD_Mat2 &Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_AddM2Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddM3Assign, 1)
static inline HMMD_Mat3 &operator+=(HMMD_Mat3 &Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_AddM3Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddM4Assign, 1)
static inline HMMD_Mat4 &operator+=(HMMD_Mat4 &Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_AddM4Assign);
    return Left = Left + Right;
}

COVERAGE(HMMD_AddQAssign, 1)
static inline HMMD_Quat &operator+=(HMMD_Quat &Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_AddQAssign);
    return Left = Left + Right;
}

COVERAGE(HMMD_SubV2Assign, 1)
static inline HMMD_Vec2 &operator-=(HMMD_Vec2 &Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_SubV2Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubV3Assign, 1)
static inline HMMD_Vec3 &operator-=(HMMD_Vec3 &Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_SubV3Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubV4Assign, 1)
static inline HMMD_Vec4 &operator-=(HMMD_Vec4 &Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_SubV4Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubM2Assign, 1)
static inline HMMD_Mat2 &operator-=(HMMD_Mat2 &Left, HMMD_Mat2 Right)
{
    ASSERT_COVERED(HMMD_SubM2Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubM3Assign, 1)
static inline HMMD_Mat3 &operator-=(HMMD_Mat3 &Left, HMMD_Mat3 Right)
{
    ASSERT_COVERED(HMMD_SubM3Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubM4Assign, 1)
static inline HMMD_Mat4 &operator-=(HMMD_Mat4 &Left, HMMD_Mat4 Right)
{
    ASSERT_COVERED(HMMD_SubM4Assign);
    return Left = Left - Right;
}

COVERAGE(HMMD_SubQAssign, 1)
static inline HMMD_Quat &operator-=(HMMD_Quat &Left, HMMD_Quat Right)
{
    ASSERT_COVERED(HMMD_SubQAssign);
    return Left = Left - Right;
}

COVERAGE(HMMD_MulV2Assign, 1)
static inline HMMD_Vec2 &operator*=(HMMD_Vec2 &Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_MulV2Assign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulV3Assign, 1)
static inline HMMD_Vec3 &operator*=(HMMD_Vec3 &Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_MulV3Assign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulV4Assign, 1)
static inline HMMD_Vec4 &operator*=(HMMD_Vec4 &Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_MulV4Assign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulV2FAssign, 1)
static inline HMMD_Vec2 &operator*=(HMMD_Vec2 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV2FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulV3FAssign, 1)
static inline HMMD_Vec3 &operator*=(HMMD_Vec3 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV3FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulV4FAssign, 1)
static inline HMMD_Vec4 &operator*=(HMMD_Vec4 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulV4FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulM2FAssign, 1)
static inline HMMD_Mat2 &operator*=(HMMD_Mat2 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM2FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulM3FAssign, 1)
static inline HMMD_Mat3 &operator*=(HMMD_Mat3 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM3FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulM4FAssign, 1)
static inline HMMD_Mat4 &operator*=(HMMD_Mat4 &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulM4FAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_MulQFAssign, 1)
static inline HMMD_Quat &operator*=(HMMD_Quat &Left, double Right)
{
    ASSERT_COVERED(HMMD_MulQFAssign);
    return Left = Left * Right;
}

COVERAGE(HMMD_DivV2Assign, 1)
static inline HMMD_Vec2 &operator/=(HMMD_Vec2 &Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_DivV2Assign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivV3Assign, 1)
static inline HMMD_Vec3 &operator/=(HMMD_Vec3 &Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_DivV3Assign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivV4Assign, 1)
static inline HMMD_Vec4 &operator/=(HMMD_Vec4 &Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_DivV4Assign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivV2FAssign, 1)
static inline HMMD_Vec2 &operator/=(HMMD_Vec2 &Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV2FAssign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivV3FAssign, 1)
static inline HMMD_Vec3 &operator/=(HMMD_Vec3 &Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV3FAssign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivV4FAssign, 1)
static inline HMMD_Vec4 &operator/=(HMMD_Vec4 &Left, double Right)
{
    ASSERT_COVERED(HMMD_DivV4FAssign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivM4FAssign, 1)
static inline HMMD_Mat4 &operator/=(HMMD_Mat4 &Left, double Right)
{
    ASSERT_COVERED(HMMD_DivM4FAssign);
    return Left = Left / Right;
}

COVERAGE(HMMD_DivQFAssign, 1)
static inline HMMD_Quat &operator/=(HMMD_Quat &Left, double Right)
{
    ASSERT_COVERED(HMMD_DivQFAssign);
    return Left = Left / Right;
}

COVERAGE(HMMD_EqV2Op, 1)
static inline HMMD_Bool operator==(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_EqV2Op);
    return HMMD_EqV2(Left, Right);
}

COVERAGE(HMMD_EqV3Op, 1)
static inline HMMD_Bool operator==(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_EqV3Op);
    return HMMD_EqV3(Left, Right);
}

COVERAGE(HMMD_EqV4Op, 1)
static inline HMMD_Bool operator==(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_EqV4Op);
    return HMMD_EqV4(Left, Right);
}

COVERAGE(HMMD_EqV2OpNot, 1)
static inline HMMD_Bool operator!=(HMMD_Vec2 Left, HMMD_Vec2 Right)
{
    ASSERT_COVERED(HMMD_EqV2OpNot);
    return !HMMD_EqV2(Left, Right);
}

COVERAGE(HMMD_EqV3OpNot, 1)
static inline HMMD_Bool operator!=(HMMD_Vec3 Left, HMMD_Vec3 Right)
{
    ASSERT_COVERED(HMMD_EqV3OpNot);
    return !HMMD_EqV3(Left, Right);
}

COVERAGE(HMMD_EqV4OpNot, 1)
static inline HMMD_Bool operator!=(HMMD_Vec4 Left, HMMD_Vec4 Right)
{
    ASSERT_COVERED(HMMD_EqV4OpNot);
    return !HMMD_EqV4(Left, Right);
}

COVERAGE(HMMD_UnaryMinusV2, 1)
static inline HMMD_Vec2 operator-(HMMD_Vec2 In)
{
    ASSERT_COVERED(HMMD_UnaryMinusV2);

    HMMD_Vec2 Result;
    Result.X = -In.X;
    Result.Y = -In.Y;

    return Result;
}

COVERAGE(HMMD_UnaryMinusV3, 1)
static inline HMMD_Vec3 operator-(HMMD_Vec3 In)
{
    ASSERT_COVERED(HMMD_UnaryMinusV3);

    HMMD_Vec3 Result;
    Result.X = -In.X;
    Result.Y = -In.Y;
    Result.Z = -In.Z;

    return Result;
}

COVERAGE(HMMD_UnaryMinusV4, 1)
static inline HMMD_Vec4 operator-(HMMD_Vec4 In)
{
    ASSERT_COVERED(HMMD_UnaryMinusV4);

    HMMD_Vec4 Result;
#if HANDMADE_MATH__USE_SSE
    Result.SSE = _mm_xor_ps(In.SSE, _mm_set1_ps(-0.0f));
#else
    Result.X = -In.X;
    Result.Y = -In.Y;
    Result.Z = -In.Z;
    Result.W = -In.W;
#endif

    return Result;
}

#endif /* __cplusplus*/

#ifdef HANDMADE_MATH__USE_C11_GENERICS
#define HMMD_Add(A, B) _Generic((A), \
        HMMD_Vec2: HMMD_AddV2, \
        HMMD_Vec3: HMMD_AddV3, \
        HMMD_Vec4: HMMD_AddV4, \
        HMMD_Mat2: HMMD_AddM2, \
        HMMD_Mat3: HMMD_AddM3, \
        HMMD_Mat4: HMMD_AddM4, \
        HMMD_Quat: HMMD_AddQ \
)(A, B)

#define HMMD_Sub(A, B) _Generic((A), \
        HMMD_Vec2: HMMD_SubV2, \
        HMMD_Vec3: HMMD_SubV3, \
        HMMD_Vec4: HMMD_SubV4, \
        HMMD_Mat2: HMMD_SubM2, \
        HMMD_Mat3: HMMD_SubM3, \
        HMMD_Mat4: HMMD_SubM4, \
        HMMD_Quat: HMMD_SubQ \
)(A, B)

#define HMMD_Mul(A, B) _Generic((B), \
     double: _Generic((A), \
        HMMD_Vec2: HMMD_MulV2F, \
        HMMD_Vec3: HMMD_MulV3F, \
        HMMD_Vec4: HMMD_MulV4F, \
        HMMD_Mat2: HMMD_MulM2F, \
        HMMD_Mat3: HMMD_MulM3F, \
        HMMD_Mat4: HMMD_MulM4F, \
        HMMD_Quat: HMMD_MulQF \
     ), \
     HMMD_Mat2: HMMD_MulM2, \
     HMMD_Mat3: HMMD_MulM3, \
     HMMD_Mat4: HMMD_MulM4, \
     HMMD_Quat: HMMD_MulQ, \
     default: _Generic((A), \
        HMMD_Vec2: HMMD_MulV2, \
        HMMD_Vec3: HMMD_MulV3, \
        HMMD_Vec4: HMMD_MulV4, \
        HMMD_Mat2: HMMD_MulM2V2, \
        HMMD_Mat3: HMMD_MulM3V3, \
        HMMD_Mat4: HMMD_MulM4V4 \
    ) \
)(A, B)

#define HMMD_Div(A, B) _Generic((B), \
     double: _Generic((A), \
        HMMD_Mat2: HMMD_DivM2F, \
        HMMD_Mat3: HMMD_DivM3F, \
        HMMD_Mat4: HMMD_DivM4F, \
        HMMD_Vec2: HMMD_DivV2F, \
        HMMD_Vec3: HMMD_DivV3F, \
        HMMD_Vec4: HMMD_DivV4F, \
        HMMD_Quat: HMMD_DivQF  \
     ), \
     HMMD_Mat2: HMMD_DivM2, \
     HMMD_Mat3: HMMD_DivM3, \
     HMMD_Mat4: HMMD_DivM4, \
     HMMD_Quat: HMMD_DivQ, \
     default: _Generic((A), \
        HMMD_Vec2: HMMD_DivV2, \
        HMMD_Vec3: HMMD_DivV3, \
        HMMD_Vec4: HMMD_DivV4  \
    ) \
)(A, B)

#define HMMD_Len(A) _Generic((A), \
        HMMD_Vec2: HMMD_LenV2, \
        HMMD_Vec3: HMMD_LenV3, \
        HMMD_Vec4: HMMD_LenV4  \
)(A)

#define HMMD_LenSqr(A) _Generic((A), \
        HMMD_Vec2: HMMD_LenSqrV2, \
        HMMD_Vec3: HMMD_LenSqrV3, \
        HMMD_Vec4: HMMD_LenSqrV4  \
)(A)

#define HMMD_Norm(A) _Generic((A), \
        HMMD_Vec2: HMMD_NormV2, \
        HMMD_Vec3: HMMD_NormV3, \
        HMMD_Vec4: HMMD_NormV4  \
)(A)

#define HMMD_Dot(A, B) _Generic((A), \
        HMMD_Vec2: HMMD_DotV2, \
        HMMD_Vec3: HMMD_DotV3, \
        HMMD_Vec4: HMMD_DotV4  \
)(A, B)

#define HMMD_Lerp(A, T, B) _Generic((A), \
        double: HMMD_Lerp, \
        HMMD_Vec2: HMMD_LerpV2, \
        HMMD_Vec3: HMMD_LerpV3, \
        HMMD_Vec4: HMMD_LerpV4 \
)(A, T, B)

#define HMMD_Eq(A, B) _Generic((A), \
        HMMD_Vec2: HMMD_EqV2, \
        HMMD_Vec3: HMMD_EqV3, \
        HMMD_Vec4: HMMD_EqV4  \
)(A, B)

#define HMMD_Transpose(M) _Generic((M), \
        HMMD_Mat2: HMMD_TransposeM2, \
        HMMD_Mat3: HMMD_TransposeM3, \
        HMMD_Mat4: HMMD_TransposeM4  \
)(M)

#define HMMD_Determinant(M) _Generic((M), \
        HMMD_Mat2: HMMD_DeterminantM2, \
        HMMD_Mat3: HMMD_DeterminantM3, \
        HMMD_Mat4: HMMD_DeterminantM4  \
)(M)

#define HMMD_InvGeneral(M) _Generic((M), \
        HMMD_Mat2: HMMD_InvGeneralM2, \
        HMMD_Mat3: HMMD_InvGeneralM3, \
        HMMD_Mat4: HMMD_InvGeneralM4  \
)(M)

#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif /* HANDMADE_MATH_H */



