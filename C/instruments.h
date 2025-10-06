#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

/**
 * @brief Assembles the complete Csound orchestra string from individual instrument definitions.
 * 
 * This function concatenates a standard header with all registered instrument
 * ORC code. The caller is responsible for freeing the returned string using free().
 *
 * @return A dynamically allocated string containing the full orchestra code,
 *         or NULL if memory allocation fails.
 */
char* get_orchestra_string();

#endif // INSTRUMENTS_H