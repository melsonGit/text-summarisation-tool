#include "../inc/DirectoryInfo.h"
#include "../inc/FileHandler.h"
#include "../inc/TextParser.h"

int main()
{
	FileHandler FileHandler (
							DirectoryInfo::inputFolderDir, 
							DirectoryInfo::stopWordsFolderDir, 
							DirectoryInfo::stopCharFolderDir, 
							DirectoryInfo::outputFolderDir, 
							DirectoryInfo::fileType
							);
	
	TextParser TextParser(FileHandler);
	TextParser.summariseFile();

	return EXIT_SUCCESS;
}

/*
* Program Flow:
*  (DONE) 1. Prompt user to enter input file name they wish to read from (e.g. inFile.txt)
*  (DONE) 2. Prompt user fo enter summarisation factor (SF) | SF = totalSummarisedWordCount: (SF / 100) * totalInputFileWordCount. (IGNORE MODULE PDF AS IT HAS INCORRECT FORMULA TO CALCULATE PERCENTAGE)
*For example, if a text file has 107 words and the user inputs a 45% SF: (45 / 100) * 107 = we only want 48 (round down, we can't have half a word) words in our summarisation
*  (DONE) 3. Read inFile.txt
*  (DONE) 4. Read stopWordFile.txt
*  (DONE) 5. Read stopCharFile.txt
* 6. Process text from inFile.txt accordingly
* 7. Output summary text to new file (e.g. outFile.txt)
* 8. Display to console some appropriate statistics
*/

/*
* Classes:
* (DONE) File - acts as file itself
* (DONE) FileHandler - open | close | edit files
* TextStatistics - collects following data:
				   least/most freq word | shortest/longest word | most removed/unremoved word | least/most freq stopWordFile encountered | summarisation factor |
*				   | total words pre and post summarisation
* TextParser - analyses text and outputs summarised text | uses TextStatistics to collect data | uses TextFilter to filter words using stopWordFile.txt
*/