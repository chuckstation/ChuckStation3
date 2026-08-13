#pragma once


namespace RSXVertex {
namespace SCALAR {
enum : u8 {
    NOP = 0x00, // No-Operation
    MOV = 0x01, // Move (copy)
    RCP = 0x02, // Reciprocal
    RCC = 0x03, // Reciprocal clamped
    RSQ = 0x04, // Reciprocal square root
    EXP = 0x05, // Exponential base 2 (low-precision)
    LOG = 0x06, // Logarithm base 2 (low-precision)
    LIT = 0x07, // Lighting calculation
    BRA = 0x08, // Branch
    BRI = 0x09, // Branch by CC register
    CAL = 0x0a, // Subroutine call
    CLI = 0x0b, // Subroutine call by CC register
    RET = 0x0c, // Return from subroutine
    LG2 = 0x0d, // Logarithm base 2
    EX2 = 0x0e, // Exponential base 2
    SIN = 0x0f, // Sine function
    COS = 0x10, // Cosine function
    BRB = 0x11, // Branch by Boolean constant
    CLB = 0x12, // Subroutine call by Boolean constant
    PSH = 0x13, // Push onto stack
    POP = 0x14, // Pop from stack
};
}   // End namespace SCALAR

namespace VECTOR {
enum : u8 {
    NOP = 0x00, // No-Operation
    MOV = 0x01, // Move
    MUL = 0x02, // Multiply
    ADD = 0x03, // Addition
    MAD = 0x04, // Multiply-Add
    DP3 = 0x05, // 3-component Dot Product
    DPH = 0x06, // Homogeneous Dot Product
    DP4 = 0x07, // 4-component Dot Product
    DST = 0x08, // Calculate distance vector
    MIN = 0x09, // Minimum
    MAX = 0x0a, // Maximum
    SLT = 0x0b, // Set-If-LessThan
    SGE = 0x0c, // Set-If-GreaterEqual
    ARL = 0x0d, // Load to address register (round down)
    FRC = 0x0e, // Extract fractional part (fraction)
    FLR = 0x0f, // Round down (floor)
    SEQ = 0x10, // Set-If-Equal
    SFL = 0x11, // Set-If-False
    SGT = 0x12, // Set-If-GreaterThan
    SLE = 0x13, // Set-If-LessEqual
    SNE = 0x14, // Set-If-NotEqual
    STR = 0x15, // Set-If-True
    SSG = 0x16, // Convert positive values to 1 and negative values to -1
    TXL = 0x19, // Texture fetch
};
}   // End namespace VECTOR
}   // End namespace RSXShaderOpcodes

namespace RSXFragment {
enum  : u8 {
    NOP = 0x00, // No-Operation
    MOV = 0x01, // Move
    MUL = 0x02, // Multiply
    ADD = 0x03, // Add
    MAD = 0x04, // Multiply-Add
    DP3 = 0x05, // 3-component Dot Product
    DP4 = 0x06, // 4-component Dot Product
    DST = 0x07, // Distance
    MIN = 0x08, // Minimum
    MAX = 0x09, // Maximum
    SLT = 0x0A, // Set-If-LessThan
    SGE = 0x0B, // Set-If-GreaterEqual
    SLE = 0x0C, // Set-If-LessEqual
    SGT = 0x0D, // Set-If-GreaterThan
    SNE = 0x0E, // Set-If-NotEqual
    SEQ = 0x0F, // Set-If-Equal
    FRC = 0x10, // Fraction (fract)
    FLR = 0x11, // Floor
    KIL = 0x12, // Kill fragment
    PK4 = 0x13, // Pack four signed 8-bit values
    UP4 = 0x14, // Unpack four signed 8-bit values
    DDX = 0x15, // Partial-derivative in x (Screen space derivative w.r.t. x)
    DDY = 0x16, // Partial-derivative in y (Screen space derivative w.r.t. y)
    TEX = 0x17, // Texture lookup
    TXP = 0x18, // Texture sample with projection (Projective texture lookup)
    TXD = 0x19, // Texture sample with partial differentiation (Texture lookup with derivatives)
    RCP = 0x1A, // Reciprocal
    RSQ = 0x1B, // Reciprocal Square Root
    EX2 = 0x1C, // Exponentiation base 2
    LG2 = 0x1D, // Log base 2
    LIT = 0x1E, // Lighting coefficients
    LRP = 0x1F, // Linear Interpolation
    STR = 0x20, // Set-If-True
    SFL = 0x21, // Set-If-False
    COS = 0x22, // Cosine
    SIN = 0x23, // Sine
    PK2 = 0x24, // Pack two 16-bit floats
    UP2 = 0x25, // Unpack two 16-bit floats
    POW = 0x26, // Power
    PKB = 0x27, // Pack bytes
    UPB = 0x28, // Unpack bytes
    PK16 = 0x29, // Pack 16 bits
    UP16 = 0x2A, // Unpack 16
    BEM = 0x2B, // Bump-environment map (a.k.a. 2D coordinate transform)
    PKG = 0x2C, // Pack with sRGB transformation
    UPG = 0x2D, // Unpack gamma
    DP2A = 0x2E, // 2-component dot product with scalar addition
    TXL = 0x2F, // Texture sample with explicit LOD
    TXB = 0x31, // Texture sample with bias
    TEXBEM = 0x33,
    TXPBEM = 0x34,
    BEMLUM = 0x35,
    REFL = 0x36, // Reflection vector
    TIMESWTEX = 0x37,
    DP2 = 0x38, // 2-component dot product
    NRM = 0x39, // Normalize
    DIV = 0x3A, // Division
    DIVSQ = 0x3B, // Divide by Square Root
    LIF = 0x3C, // Final part of LIT
    FENCT = 0x3D, // Fence T?
    FENCB = 0x3E, // Fence B?
    BRK = 0x40, // Break
    CAL = 0x41, // Subroutine call
    IFE = 0x42, // If
    LOOP = 0x43, // Loop
    REP = 0x44, // Repeat
    RET = 0x45  // Return
};
}   // End namespace RSXFragment