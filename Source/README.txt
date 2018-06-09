Markov.exe, by Jonathan Roop
version 1.0

I made this simple Windows program for generating and displaying Markov Chains from selected text documents. Simply add files and click "Generate" to make your very own Markov chain.

No installation is necessary to use this program - simply double-click on the .exe file to run it. Since the program was written using C++ and directly uses Win32 API calls, it should be able to run on Windows systems as far back as Windows 95.

Note: The text files read by this program are assumed to use UTF-8 encoding.

--------------------You May Use This Code-------------------- 

I have made the source code to this program available so that prospective employers can see how pretty my code is. Even if you're not an employer, however, feel free to use, modify, and redistribute this code; just be sure to give me credit somewhere. 


--------------------What *Is* A Markov Chain?--------------------

In most literature, certain words or letters tends to appear after certain sequences of other words and letters. For example, the words "Rosencrantz and" tend to be followed by "Guildestern" in Shakespeare's Hamlet. A Markov chain is generated text that attempts to mimic the writing of a given document by concatenating words that tend to occur together in that document. 

Markov chains have an associated "order." Consider the following text:
"I am not a filly. I mean, I am not a squirrel! I am an old mare, understand?"

A 2nd-order Markov chain would examine two words in the text at a time and observe which word follows. We can construct a table of 2-word "prefixes" and associated 1-word "suffixes" as follows:

Prefix:			Suffixes:
I am            not; not; a
am not          a; a
not a           filly.; squirrel!
a filly.        I
filly. I        mean
I mean,         I
mean, I         am
a squirrel!     I
squirrel! I     am
am an           old
an old          mare,
old mare,       understand?

The generated text is started by picking a random prefix. The next word in the generated text is then chosen by randomly picking one word from the list of possible suffixes for that prefix. Now the last two words in the generated text make a different prefix. The next word is chosen by randomly picking from the list of possible suffixes for *that* prefix. And so on. The result might look something like this:

"old mare, understand? I am not a filly. I mean, I am not a filly. I mean, I am not a squirrel! I am not a squirrel! I am not a squirrel! I am not a filly. I mean, I am not a filly. I mean, I am an old mare, understand? I am not a filly."

Note that, in order to ensure that text can continue to be generated indefinitely, the following entry was also added to the table to make the chain "wrap" from the end of the source text back to its beginning:
Prefix:				Suffix:
mare, understand?   I

A higher-order Markov chain uses longer prefixes in its table. In general, higher-order chains result in generated text that is closer to the original source text. If the order is too high, however, the output will be identical to the source. 

Markov chains don't need to be strictly based on words; they can also be based on letters. Sophisticated algorithms can even take into account parts of speech. In the Advanced Options dialogbox of this program, the user may specify whether the program should generate text based on words or on characters. 


--------------------Known Bugs and Areas For Improvement--------------------

There are only two bugs in the program that I'm aware of. First, it's possible for the generated text to omit the space character that should exist between a punctuation mark and the next sentence. Fixing it would require some special-case code that would take up a disproportionate amount of space and make my code less readable, so I've opted to just leave it there. Here's an explanation of the bug: 

It is likely that the very last character of a given text document is a punctuation mark. However, the markov chain wraps from the end of the source back to its beginning, which is likely to be a non-whitespace character. Hence, there will exist a prefix that ends in, say, '.' that has a suffix like 'T' and may result in generated text with mashed-together sentences like: "This is an example sentence.This is another sentence." 

Second, filenames with UTF-8 encoding are not displayed properly in the ListBox. This could possibly be fixed by changing the default font used by the ListBox. The program will still generate good output, however. 


I've also identified a couple of things that can be done to improve the program. Perhaps I'll implement these changes one day if I'm bored:

1. Include support for other encodings, such as UTF-16. Since the encoding of a unicode document cannot be reliably determined from the Byte-Order Mark, a good algorithm for guessing the correct encoding would need to be imported from a 3rd party library.
2. Don't regenerate the entire Markov chain table if the source texts and Markov order have not changed (saves a little bit of time for large input files). Better yet, store a different chain table for each file, and link together common prefixes. This way, some of the files can be removed and the markov chain table wouldn't need to be regenerated. 
3. Add hotkeys 
4. Strip out header info in text documents like .docx, so that gibberish is only generated from user text.  














ToDo:
disclaimer on bugs
where available on github
contact info