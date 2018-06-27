# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

A 45 letter word, which is the maximum length that can be checked by speller. 
This word would be a good word to use to test the speller if we were writing unit tests for the code.

## According to its man page, what does `getrusage` do?

It collects resource usage data

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Because an rusage struct is really big. Don't want to have to make new copies of rusuage and pass them to the calculate function. 
It's better to tell calculate where to find the rusage structs by passing a reference to them.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

It starts reading in each character from the file. It checks to that the character is either alphabetic, or an apostrophe thats not the first character of a word. 
It adds the character to a word array, making sure that the word array is not longer than the maximum length of 45 characters.
Words longer than 45 character are thrown away.
If the character read hits a character which is a number, it will also throw the word away.
If the character read hits a blank space, it knows that the word is complete. It then updates the word counter by incremementing it by one.
It then checks the spelling of the word it has found.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

It's more accurate to check each character individually to identify words. fscanf identifies words by splitting strings on whitespaces.
fscanf would recognise garbage values containing numbers and special characters as words if they were separated by whitespaces.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Constants are variables that can't be changed. The word being checked and the dictionary used to check the word are not expected to change during checking or loading tasks.
I think the parameters have been declared as constants for safety reasons. To avoid changing something which should never be changed.

