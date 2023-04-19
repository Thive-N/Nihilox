#ifndef NIHILOX_TOKEN_ITERATOR_H
#define NIHILOX_TOKEN_ITERATOR_H

#include <string>
#include <vector>

template<typename Type> class token_iterator {
  public:
	explicit token_iterator(std::vector<Type> &tokens_)
	{
		tokens = std::move(tokens_); // move the tokens_ to the parser class
		begin  = tokens.begin();	 // set the begin iterator to the beginning of the vector
		end	   = tokens.end();		 // set the end iterator to the end of the vector
	}

	/// @return The next token in the vector
	/// @note The iterator is incremented by one but moved back to the original position
	Type peek()
	{
		if (notAtStart) {
			begin++;
			auto temp = *begin;
			begin--;
			return temp;
		}
		return *begin;
	}

	/// @return The next token in the vector
	/// @note The iterator is incremented by one
	Type next()
	{
		if (notAtStart) {
			begin++;	   // increment the iterator
			return *begin; // return the token
		}
		notAtStart = true;
		return *begin;
	}

	/// @note moves the iterator back by one
	/// useful in expression parsing so that no more than the required tokens are consumed
	void back() { begin--; }

	/// @return if the iterator is at the end of the vector
	/// @note This will check if the iterator is at the end of the vector or beyond
	bool isAtEnd() { return (begin != tokens.end() && begin + 1 == tokens.end()); }

	/// @return The current token
	/// @note This will return the current token regardless of where the iterator is
	Type getToken() { return *begin; }

  private:
	typename std::vector<Type>::const_iterator begin;
	typename std::vector<Type>::const_iterator end;
	std::vector<Type>						   tokens;
	bool									   notAtStart = false;
};

#endif // NIHILOX_TOKEN_ITERATOR_H
