#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>

/* Testcase:

achieve my advise -> put my tell
my extraordinary walk is gradual -> my amazing move is slow
the evil diagram -> the ugly plan
I consider a microscopic cloak to be irritable -> I think a little hide to be moody

 */

int main()
{
  // 1. open SYNONYMS
  std::ifstream ifs{"SYNONYMS.txt"};

  // 2. read the synonyms into the synonyms map
  std::map<std::string, std::string> synonyms{};
  std::transform(std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{},
    std::inserter(synonyms, synonyms.begin()),
    [](std::string const& line) 
    {
      auto it = std::find(std::begin(line), std::end(line), '=');
      return std::make_pair(std::string(std::begin(line), it), std::string(std::next(it), std::end(line)));

    });
  // 3. read text from cin into the vector words, replacing each
  //    possible word, leaving the rest as-is

  std::vector<std::string> words{};
  std::transform(std::istream_iterator<std::string>{std::cin}, std::istream_iterator<std::string>{},
    std::back_inserter(words),
    [&synonyms](std::string const& word) 
    {
      if (synonyms.find(word) != synonyms.end())
        return synonyms[word];
      return word;
    });

  // 4. print the text stored in words

  // for (auto [key, value] : synonyms)
  // {
  //   std::cout << key << " " << value << std::endl;
  // }

  std::copy(words.begin(), words.end(), std::ostream_iterator<std::string>(std::cout," "));
  std::cout << std::endl;
}
