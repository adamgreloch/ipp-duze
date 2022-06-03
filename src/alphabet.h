/** @file
 * Interfejs klasy definiującej alfabet.
 *
 * \e Alfabetem nazywamy znaki ze zbioru \f$\Omega=\{0,1,\ldots,9\}\cup\{*,
 * \#\}\f$.
 * Niepuste ciągi znaków z alfabetu nazywamy odtąd <em> poprawnymi ciągami
 * znaków </em>.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __ALPHABET_H__
#define __ALPHABET_H__

#define ALLNUM 12 /**< Rozmiar alfabetu w drzewie,
                       czyli moc zbioru \f$\Omega\f$. */

/**
 * @brief Zwraca wartość liczbową znaku @p c, jeśli należy do alfabetu.
 * @param[in] c - sprawdzany znak.
 * @return Wartość liczbowa z przedziału od @p 0 do @p 11, jeśli znak należy do
 * alfabetu, lub @p -1, jeśli podano znak spoza alfabetu.
 */
int getValue(char c);

/**
 * @brief Sprawdza, czy @p str jest poprawnym ciągiem znaków.
 * Sprawdza, czy ciąg znaków @p str jest poprawny z definicji alfabetu oraz
 * czy jest zakończony znakiem terminującym '\0'. Liczy długość numeru.
 * @param[in] str - sprawdzany ciąg znaków.
 * @return Dodatnia wartość liczbowa reprezentująca długość numeru, jeśli @p
 * str jest poprawny, lub zero przeciwnym wypadku.
 */
size_t isCorrect(char const *str);

/**
 * @brief Przeprowadza porównanie dwóch poprawnych ciągów znaku w porządku
 * leksykograficznym.
 * Rosnący ciąg \f$\langle 0,1,2,\ldots,9,*,\# \rangle\f$ definiuje
 * leksykograficzny porządek na tym alfabecie.
 * @param[in] a - podwójny wskaźnik na ciąg znaków reprezentujący pierwszy numer
 * @param[in] b - podwójny wskaźnik na ciąg znaków reprezentujący drugi numer
 * @return Wartość ujemna, jeśli @p a jest mniejszy od @p b. Wartość zero,
 * jeśli @p a jest równe @p b. Wartość dodatnia, jeśli @p a jest większy od
 */
int strCompare(const void *a, const void *b);

#endif /* __ALPHABET_H__ */
