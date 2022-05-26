#include <iostream>

int main()
{
	return 0;
}

/*
* Program Flow:
* 1. Prompt user to enter input file name (e.g. inFile.txt)
* 2. Prompt user fo enter summarisation factor (SF) | SF = totalSummarisedWordCount: (SF / 100) * totalInputFileWordCount. (IGNORE MODULE PDF AS IT HAS INCORRECT FORMULA TO CALCULATE PERCENTAGE)
*For example, if a text file has 107 words and the user inputs a 45% SF: (45 / 100) * 107 = we only want 48 (round down, we can't have half a word) words in our summarisation
* 3. Read inFile.txt
* 4. Read stopWords.txt
* 5. Process text from inFile.txt accordingly
* 6. Output summary text to new file (e.g. outFile.txt)
* 7. Display to console some appropriate statistics
*/

/*
* Classes:
* FileHandler - open | close | edit files
* TextStatistics - collects following data:
				   least/most freq word and letter | shortest/longest word | most removed/unremoved word | least/most freq stopWords encountered | summarisation factor |
*				   sentence with most/least words pre and post summarisation | total words pre and post summarisation
* TextSummariser - analyses text and outputs summarised text | uses TextStatistics to collect data | uses TextFilter to filter words using stopWords.txt
* TextFilter - filters text using stopWords.txt
*/