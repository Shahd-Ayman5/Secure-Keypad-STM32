#ifndef BIT_MATH_H_
#define BIT_MATH_H_

/* Single Bit Operations */
#define SET_BIT(REG, POS)                    ((REG) |=  (1U << (POS)))
#define CLR_BIT(REG, POS)                    ((REG) &= ~(1U << (POS)))
#define TOG_BIT(REG, POS)                    ((REG) ^=  (1U << (POS)))
#define READ_BIT(REG, POS)                   (((REG) & (1U << (POS))) >> (POS))

/* Mask Operations */
#define SET_MASK(REG, MASK)                  ((REG) |=  (MASK))
#define CLR_MASK(REG, MASK)                  ((REG) &= ~(MASK))

/* Bit-Field Operations */
#define WRITE_FIELD(REG, BASE_MASK, POS, VAL)      ((REG) = (((REG) & ~((BASE_MASK) << (POS))) | (((VAL) & (BASE_MASK)) << (POS))))

#define READ_FIELD(REG, BASE_MASK, POS)          (((REG) >> (POS)) & (BASE_MASK))

#endif /* BIT_MATH_H_ */
