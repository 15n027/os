/* Generated automatically from machmode.def and config/i386/i386-modes.def
   by genmodes.  */

#ifndef GCC_INSN_MODES_INLINE_H
#define GCC_INSN_MODES_INLINE_H

#if !defined (USED_FOR_TARGET) && GCC_VERSION >= 4001

#ifdef __cplusplus
inline __attribute__((__always_inline__))
#else
extern __inline__ __attribute__((__always_inline__, __gnu_inline__))
#endif
unsigned short
mode_size_inline (machine_mode mode)
{
  extern unsigned short mode_size[NUM_MACHINE_MODES];
  gcc_assert (mode >= 0 && mode < NUM_MACHINE_MODES);
  switch (mode)
    {
    case VOIDmode: return 0;
    case BLKmode: return 0;
    case CCmode: return 4;
    case CCGCmode: return 4;
    case CCGOCmode: return 4;
    case CCNOmode: return 4;
    case CCAmode: return 4;
    case CCCmode: return 4;
    case CCOmode: return 4;
    case CCPmode: return 4;
    case CCSmode: return 4;
    case CCZmode: return 4;
    case CCFPmode: return 4;
    case CCFPUmode: return 4;
    case BImode: return 1;
    case QImode: return 1;
    case HImode: return 2;
    case SImode: return 4;
    case DImode: return 8;
    case TImode: return 16;
    case OImode: return 32;
    case XImode: return 64;
    case BND32mode: return 8;
    case BND64mode: return 16;
    case QQmode: return 1;
    case HQmode: return 2;
    case SQmode: return 4;
    case DQmode: return 8;
    case TQmode: return 16;
    case UQQmode: return 1;
    case UHQmode: return 2;
    case USQmode: return 4;
    case UDQmode: return 8;
    case UTQmode: return 16;
    case HAmode: return 2;
    case SAmode: return 4;
    case DAmode: return 8;
    case TAmode: return 16;
    case UHAmode: return 2;
    case USAmode: return 4;
    case UDAmode: return 8;
    case UTAmode: return 16;
    case SFmode: return 4;
    case DFmode: return 8;
    case TFmode: return 16;
    case SDmode: return 4;
    case DDmode: return 8;
    case TDmode: return 16;
    case CQImode: return 2;
    case CHImode: return 4;
    case CSImode: return 8;
    case CDImode: return 16;
    case CTImode: return 32;
    case COImode: return 64;
    case CXImode: return 128;
    case SCmode: return 8;
    case DCmode: return 16;
    case TCmode: return 32;
    case V2QImode: return 2;
    case V4QImode: return 4;
    case V2HImode: return 4;
    case V1SImode: return 4;
    case V8QImode: return 8;
    case V4HImode: return 8;
    case V2SImode: return 8;
    case V1DImode: return 8;
    case V12QImode: return 12;
    case V6HImode: return 12;
    case V14QImode: return 14;
    case V16QImode: return 16;
    case V8HImode: return 16;
    case V4SImode: return 16;
    case V2DImode: return 16;
    case V1TImode: return 16;
    case V32QImode: return 32;
    case V16HImode: return 32;
    case V8SImode: return 32;
    case V4DImode: return 32;
    case V2TImode: return 32;
    case V64QImode: return 64;
    case V32HImode: return 64;
    case V16SImode: return 64;
    case V8DImode: return 64;
    case V4TImode: return 64;
    case V128QImode: return 128;
    case V64HImode: return 128;
    case V32SImode: return 128;
    case V16DImode: return 128;
    case V8TImode: return 128;
    case V64SImode: return 256;
    case V2SFmode: return 8;
    case V4SFmode: return 16;
    case V2DFmode: return 16;
    case V8SFmode: return 32;
    case V4DFmode: return 32;
    case V2TFmode: return 32;
    case V16SFmode: return 64;
    case V8DFmode: return 64;
    case V4TFmode: return 64;
    case V32SFmode: return 128;
    case V16DFmode: return 128;
    case V8TFmode: return 128;
    case V64SFmode: return 256;
    case V32DFmode: return 256;
    case V16TFmode: return 256;
    default: return mode_size[mode];
    }
}

#ifdef __cplusplus
inline __attribute__((__always_inline__))
#else
extern __inline__ __attribute__((__always_inline__, __gnu_inline__))
#endif
unsigned char
mode_nunits_inline (machine_mode mode)
{
  extern const unsigned char mode_nunits[NUM_MACHINE_MODES];
  gcc_assert (mode >= 0 && mode < NUM_MACHINE_MODES);
  switch (mode)
    {
    case VOIDmode: return 0;
    case BLKmode: return 0;
    case CCmode: return 1;
    case CCGCmode: return 1;
    case CCGOCmode: return 1;
    case CCNOmode: return 1;
    case CCAmode: return 1;
    case CCCmode: return 1;
    case CCOmode: return 1;
    case CCPmode: return 1;
    case CCSmode: return 1;
    case CCZmode: return 1;
    case CCFPmode: return 1;
    case CCFPUmode: return 1;
    case BImode: return 1;
    case QImode: return 1;
    case HImode: return 1;
    case SImode: return 1;
    case DImode: return 1;
    case TImode: return 1;
    case OImode: return 1;
    case XImode: return 1;
    case BND32mode: return 1;
    case BND64mode: return 1;
    case QQmode: return 1;
    case HQmode: return 1;
    case SQmode: return 1;
    case DQmode: return 1;
    case TQmode: return 1;
    case UQQmode: return 1;
    case UHQmode: return 1;
    case USQmode: return 1;
    case UDQmode: return 1;
    case UTQmode: return 1;
    case HAmode: return 1;
    case SAmode: return 1;
    case DAmode: return 1;
    case TAmode: return 1;
    case UHAmode: return 1;
    case USAmode: return 1;
    case UDAmode: return 1;
    case UTAmode: return 1;
    case SFmode: return 1;
    case DFmode: return 1;
    case XFmode: return 1;
    case TFmode: return 1;
    case SDmode: return 1;
    case DDmode: return 1;
    case TDmode: return 1;
    case CQImode: return 2;
    case CHImode: return 2;
    case CSImode: return 2;
    case CDImode: return 2;
    case CTImode: return 2;
    case COImode: return 2;
    case CXImode: return 2;
    case SCmode: return 2;
    case DCmode: return 2;
    case XCmode: return 2;
    case TCmode: return 2;
    case V2QImode: return 2;
    case V4QImode: return 4;
    case V2HImode: return 2;
    case V1SImode: return 1;
    case V8QImode: return 8;
    case V4HImode: return 4;
    case V2SImode: return 2;
    case V1DImode: return 1;
    case V12QImode: return 12;
    case V6HImode: return 6;
    case V14QImode: return 14;
    case V16QImode: return 16;
    case V8HImode: return 8;
    case V4SImode: return 4;
    case V2DImode: return 2;
    case V1TImode: return 1;
    case V32QImode: return 32;
    case V16HImode: return 16;
    case V8SImode: return 8;
    case V4DImode: return 4;
    case V2TImode: return 2;
    case V64QImode: return 64;
    case V32HImode: return 32;
    case V16SImode: return 16;
    case V8DImode: return 8;
    case V4TImode: return 4;
    case V128QImode: return 128;
    case V64HImode: return 64;
    case V32SImode: return 32;
    case V16DImode: return 16;
    case V8TImode: return 8;
    case V64SImode: return 64;
    case V2SFmode: return 2;
    case V4SFmode: return 4;
    case V2DFmode: return 2;
    case V8SFmode: return 8;
    case V4DFmode: return 4;
    case V2TFmode: return 2;
    case V16SFmode: return 16;
    case V8DFmode: return 8;
    case V4TFmode: return 4;
    case V32SFmode: return 32;
    case V16DFmode: return 16;
    case V8TFmode: return 8;
    case V64SFmode: return 64;
    case V32DFmode: return 32;
    case V16TFmode: return 16;
    default: return mode_nunits[mode];
    }
}

#ifdef __cplusplus
inline __attribute__((__always_inline__))
#else
extern __inline__ __attribute__((__always_inline__, __gnu_inline__))
#endif
unsigned char
mode_inner_inline (machine_mode mode)
{
  extern const unsigned char mode_inner[NUM_MACHINE_MODES];
  gcc_assert (mode >= 0 && mode < NUM_MACHINE_MODES);
  switch (mode)
    {
    case VOIDmode: return VOIDmode;
    case BLKmode: return BLKmode;
    case CCmode: return CCmode;
    case CCGCmode: return CCGCmode;
    case CCGOCmode: return CCGOCmode;
    case CCNOmode: return CCNOmode;
    case CCAmode: return CCAmode;
    case CCCmode: return CCCmode;
    case CCOmode: return CCOmode;
    case CCPmode: return CCPmode;
    case CCSmode: return CCSmode;
    case CCZmode: return CCZmode;
    case CCFPmode: return CCFPmode;
    case CCFPUmode: return CCFPUmode;
    case BImode: return BImode;
    case QImode: return QImode;
    case HImode: return HImode;
    case SImode: return SImode;
    case DImode: return DImode;
    case TImode: return TImode;
    case OImode: return OImode;
    case XImode: return XImode;
    case BND32mode: return BND32mode;
    case BND64mode: return BND64mode;
    case QQmode: return QQmode;
    case HQmode: return HQmode;
    case SQmode: return SQmode;
    case DQmode: return DQmode;
    case TQmode: return TQmode;
    case UQQmode: return UQQmode;
    case UHQmode: return UHQmode;
    case USQmode: return USQmode;
    case UDQmode: return UDQmode;
    case UTQmode: return UTQmode;
    case HAmode: return HAmode;
    case SAmode: return SAmode;
    case DAmode: return DAmode;
    case TAmode: return TAmode;
    case UHAmode: return UHAmode;
    case USAmode: return USAmode;
    case UDAmode: return UDAmode;
    case UTAmode: return UTAmode;
    case SFmode: return SFmode;
    case DFmode: return DFmode;
    case XFmode: return XFmode;
    case TFmode: return TFmode;
    case SDmode: return SDmode;
    case DDmode: return DDmode;
    case TDmode: return TDmode;
    case CQImode: return QImode;
    case CHImode: return HImode;
    case CSImode: return SImode;
    case CDImode: return DImode;
    case CTImode: return TImode;
    case COImode: return OImode;
    case CXImode: return XImode;
    case SCmode: return SFmode;
    case DCmode: return DFmode;
    case XCmode: return XFmode;
    case TCmode: return TFmode;
    case V2QImode: return QImode;
    case V4QImode: return QImode;
    case V2HImode: return HImode;
    case V1SImode: return SImode;
    case V8QImode: return QImode;
    case V4HImode: return HImode;
    case V2SImode: return SImode;
    case V1DImode: return DImode;
    case V12QImode: return QImode;
    case V6HImode: return HImode;
    case V14QImode: return QImode;
    case V16QImode: return QImode;
    case V8HImode: return HImode;
    case V4SImode: return SImode;
    case V2DImode: return DImode;
    case V1TImode: return TImode;
    case V32QImode: return QImode;
    case V16HImode: return HImode;
    case V8SImode: return SImode;
    case V4DImode: return DImode;
    case V2TImode: return TImode;
    case V64QImode: return QImode;
    case V32HImode: return HImode;
    case V16SImode: return SImode;
    case V8DImode: return DImode;
    case V4TImode: return TImode;
    case V128QImode: return QImode;
    case V64HImode: return HImode;
    case V32SImode: return SImode;
    case V16DImode: return DImode;
    case V8TImode: return TImode;
    case V64SImode: return SImode;
    case V2SFmode: return SFmode;
    case V4SFmode: return SFmode;
    case V2DFmode: return DFmode;
    case V8SFmode: return SFmode;
    case V4DFmode: return DFmode;
    case V2TFmode: return TFmode;
    case V16SFmode: return SFmode;
    case V8DFmode: return DFmode;
    case V4TFmode: return TFmode;
    case V32SFmode: return SFmode;
    case V16DFmode: return DFmode;
    case V8TFmode: return TFmode;
    case V64SFmode: return SFmode;
    case V32DFmode: return DFmode;
    case V16TFmode: return TFmode;
    default: return mode_inner[mode];
    }
}

#ifdef __cplusplus
inline __attribute__((__always_inline__))
#else
extern __inline__ __attribute__((__always_inline__, __gnu_inline__))
#endif
unsigned char
mode_unit_size_inline (machine_mode mode)
{
  extern CONST_MODE_UNIT_SIZE unsigned char mode_unit_size[NUM_MACHINE_MODES];
  gcc_assert (mode >= 0 && mode < NUM_MACHINE_MODES);
  switch (mode)
    {
    case VOIDmode: return 0;
    case BLKmode: return 0;
    case CCmode: return 4;
    case CCGCmode: return 4;
    case CCGOCmode: return 4;
    case CCNOmode: return 4;
    case CCAmode: return 4;
    case CCCmode: return 4;
    case CCOmode: return 4;
    case CCPmode: return 4;
    case CCSmode: return 4;
    case CCZmode: return 4;
    case CCFPmode: return 4;
    case CCFPUmode: return 4;
    case BImode: return 1;
    case QImode: return 1;
    case HImode: return 2;
    case SImode: return 4;
    case DImode: return 8;
    case TImode: return 16;
    case OImode: return 32;
    case XImode: return 64;
    case BND32mode: return 8;
    case BND64mode: return 16;
    case QQmode: return 1;
    case HQmode: return 2;
    case SQmode: return 4;
    case DQmode: return 8;
    case TQmode: return 16;
    case UQQmode: return 1;
    case UHQmode: return 2;
    case USQmode: return 4;
    case UDQmode: return 8;
    case UTQmode: return 16;
    case HAmode: return 2;
    case SAmode: return 4;
    case DAmode: return 8;
    case TAmode: return 16;
    case UHAmode: return 2;
    case USAmode: return 4;
    case UDAmode: return 8;
    case UTAmode: return 16;
    case SFmode: return 4;
    case DFmode: return 8;
    case TFmode: return 16;
    case SDmode: return 4;
    case DDmode: return 8;
    case TDmode: return 16;
    case CQImode: return 1;
    case CHImode: return 2;
    case CSImode: return 4;
    case CDImode: return 8;
    case CTImode: return 16;
    case COImode: return 32;
    case CXImode: return 64;
    case SCmode: return 4;
    case DCmode: return 8;
    case TCmode: return 16;
    case V2QImode: return 1;
    case V4QImode: return 1;
    case V2HImode: return 2;
    case V1SImode: return 4;
    case V8QImode: return 1;
    case V4HImode: return 2;
    case V2SImode: return 4;
    case V1DImode: return 8;
    case V12QImode: return 1;
    case V6HImode: return 2;
    case V14QImode: return 1;
    case V16QImode: return 1;
    case V8HImode: return 2;
    case V4SImode: return 4;
    case V2DImode: return 8;
    case V1TImode: return 16;
    case V32QImode: return 1;
    case V16HImode: return 2;
    case V8SImode: return 4;
    case V4DImode: return 8;
    case V2TImode: return 16;
    case V64QImode: return 1;
    case V32HImode: return 2;
    case V16SImode: return 4;
    case V8DImode: return 8;
    case V4TImode: return 16;
    case V128QImode: return 1;
    case V64HImode: return 2;
    case V32SImode: return 4;
    case V16DImode: return 8;
    case V8TImode: return 16;
    case V64SImode: return 4;
    case V2SFmode: return 4;
    case V4SFmode: return 4;
    case V2DFmode: return 8;
    case V8SFmode: return 4;
    case V4DFmode: return 8;
    case V2TFmode: return 16;
    case V16SFmode: return 4;
    case V8DFmode: return 8;
    case V4TFmode: return 16;
    case V32SFmode: return 4;
    case V16DFmode: return 8;
    case V8TFmode: return 16;
    case V64SFmode: return 4;
    case V32DFmode: return 8;
    case V16TFmode: return 16;
    default: return mode_unit_size[mode];
    }
}

#ifdef __cplusplus
inline __attribute__((__always_inline__))
#else
extern __inline__ __attribute__((__always_inline__, __gnu_inline__))
#endif
unsigned short
mode_unit_precision_inline (machine_mode mode)
{
  extern const unsigned short mode_unit_precision[NUM_MACHINE_MODES];
  gcc_assert (mode >= 0 && mode < NUM_MACHINE_MODES);
  switch (mode)
    {
    case VOIDmode: return 0;
    case BLKmode: return 0;
    case CCmode: return 4*BITS_PER_UNIT;
    case CCGCmode: return 4*BITS_PER_UNIT;
    case CCGOCmode: return 4*BITS_PER_UNIT;
    case CCNOmode: return 4*BITS_PER_UNIT;
    case CCAmode: return 4*BITS_PER_UNIT;
    case CCCmode: return 4*BITS_PER_UNIT;
    case CCOmode: return 4*BITS_PER_UNIT;
    case CCPmode: return 4*BITS_PER_UNIT;
    case CCSmode: return 4*BITS_PER_UNIT;
    case CCZmode: return 4*BITS_PER_UNIT;
    case CCFPmode: return 4*BITS_PER_UNIT;
    case CCFPUmode: return 4*BITS_PER_UNIT;
    case BImode: return 1;
    case QImode: return 1*BITS_PER_UNIT;
    case HImode: return 2*BITS_PER_UNIT;
    case SImode: return 4*BITS_PER_UNIT;
    case DImode: return 8*BITS_PER_UNIT;
    case TImode: return 16*BITS_PER_UNIT;
    case OImode: return 32*BITS_PER_UNIT;
    case XImode: return 64*BITS_PER_UNIT;
    case BND32mode: return 8*BITS_PER_UNIT;
    case BND64mode: return 16*BITS_PER_UNIT;
    case QQmode: return 1*BITS_PER_UNIT;
    case HQmode: return 2*BITS_PER_UNIT;
    case SQmode: return 4*BITS_PER_UNIT;
    case DQmode: return 8*BITS_PER_UNIT;
    case TQmode: return 16*BITS_PER_UNIT;
    case UQQmode: return 1*BITS_PER_UNIT;
    case UHQmode: return 2*BITS_PER_UNIT;
    case USQmode: return 4*BITS_PER_UNIT;
    case UDQmode: return 8*BITS_PER_UNIT;
    case UTQmode: return 16*BITS_PER_UNIT;
    case HAmode: return 2*BITS_PER_UNIT;
    case SAmode: return 4*BITS_PER_UNIT;
    case DAmode: return 8*BITS_PER_UNIT;
    case TAmode: return 16*BITS_PER_UNIT;
    case UHAmode: return 2*BITS_PER_UNIT;
    case USAmode: return 4*BITS_PER_UNIT;
    case UDAmode: return 8*BITS_PER_UNIT;
    case UTAmode: return 16*BITS_PER_UNIT;
    case SFmode: return 4*BITS_PER_UNIT;
    case DFmode: return 8*BITS_PER_UNIT;
    case XFmode: return 80;
    case TFmode: return 16*BITS_PER_UNIT;
    case SDmode: return 4*BITS_PER_UNIT;
    case DDmode: return 8*BITS_PER_UNIT;
    case TDmode: return 16*BITS_PER_UNIT;
    case CQImode: return 1*BITS_PER_UNIT;
    case CHImode: return 2*BITS_PER_UNIT;
    case CSImode: return 4*BITS_PER_UNIT;
    case CDImode: return 8*BITS_PER_UNIT;
    case CTImode: return 16*BITS_PER_UNIT;
    case COImode: return 32*BITS_PER_UNIT;
    case CXImode: return 64*BITS_PER_UNIT;
    case SCmode: return 4*BITS_PER_UNIT;
    case DCmode: return 8*BITS_PER_UNIT;
    case XCmode: return 80;
    case TCmode: return 16*BITS_PER_UNIT;
    case V2QImode: return 1*BITS_PER_UNIT;
    case V4QImode: return 1*BITS_PER_UNIT;
    case V2HImode: return 2*BITS_PER_UNIT;
    case V1SImode: return 4*BITS_PER_UNIT;
    case V8QImode: return 1*BITS_PER_UNIT;
    case V4HImode: return 2*BITS_PER_UNIT;
    case V2SImode: return 4*BITS_PER_UNIT;
    case V1DImode: return 8*BITS_PER_UNIT;
    case V12QImode: return 1*BITS_PER_UNIT;
    case V6HImode: return 2*BITS_PER_UNIT;
    case V14QImode: return 1*BITS_PER_UNIT;
    case V16QImode: return 1*BITS_PER_UNIT;
    case V8HImode: return 2*BITS_PER_UNIT;
    case V4SImode: return 4*BITS_PER_UNIT;
    case V2DImode: return 8*BITS_PER_UNIT;
    case V1TImode: return 16*BITS_PER_UNIT;
    case V32QImode: return 1*BITS_PER_UNIT;
    case V16HImode: return 2*BITS_PER_UNIT;
    case V8SImode: return 4*BITS_PER_UNIT;
    case V4DImode: return 8*BITS_PER_UNIT;
    case V2TImode: return 16*BITS_PER_UNIT;
    case V64QImode: return 1*BITS_PER_UNIT;
    case V32HImode: return 2*BITS_PER_UNIT;
    case V16SImode: return 4*BITS_PER_UNIT;
    case V8DImode: return 8*BITS_PER_UNIT;
    case V4TImode: return 16*BITS_PER_UNIT;
    case V128QImode: return 1*BITS_PER_UNIT;
    case V64HImode: return 2*BITS_PER_UNIT;
    case V32SImode: return 4*BITS_PER_UNIT;
    case V16DImode: return 8*BITS_PER_UNIT;
    case V8TImode: return 16*BITS_PER_UNIT;
    case V64SImode: return 4*BITS_PER_UNIT;
    case V2SFmode: return 4*BITS_PER_UNIT;
    case V4SFmode: return 4*BITS_PER_UNIT;
    case V2DFmode: return 8*BITS_PER_UNIT;
    case V8SFmode: return 4*BITS_PER_UNIT;
    case V4DFmode: return 8*BITS_PER_UNIT;
    case V2TFmode: return 16*BITS_PER_UNIT;
    case V16SFmode: return 4*BITS_PER_UNIT;
    case V8DFmode: return 8*BITS_PER_UNIT;
    case V4TFmode: return 16*BITS_PER_UNIT;
    case V32SFmode: return 4*BITS_PER_UNIT;
    case V16DFmode: return 8*BITS_PER_UNIT;
    case V8TFmode: return 16*BITS_PER_UNIT;
    case V64SFmode: return 4*BITS_PER_UNIT;
    case V32DFmode: return 8*BITS_PER_UNIT;
    case V16TFmode: return 16*BITS_PER_UNIT;
    default: return mode_unit_precision[mode];
    }
}

#endif /* GCC_VERSION >= 4001 */

#endif /* insn-modes-inline.h */
