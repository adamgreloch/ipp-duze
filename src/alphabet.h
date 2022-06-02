#ifndef __ALPHABET_H__
#define __ALPHABET_H__

#define ALLNUM 12 /**< Rozmiar alfabetu w drzewie,
                       czyli moc zbioru \f$\Omega\f$. */

int getValue(char c);

/**
 * @brief Sprawdza, czy @p str jest poprawnym ciągiem znaków.
 * Sprawdza, czy ciąg znaków @p str jest poprawny tzn.
 * czy jest niepustym ciągiem złożonym wyłącznie ze znaków \f$0,1,...,9, #, *
 * \f$ i * zakończonym znakiem terminującym '\0'. Liczy długość numeru.
 * @param[in] str - sprawdzany ciąg znaków.
 * @return Dodatnia wartość liczbowa reprezentująca długość numeru, jeśli @p
 * str jest numerem telefonu lub zero przeciwnym wypadku.
 */
size_t isCorrect(char const *str);

#endif /* __ALPHABET_H__ */
