#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Book
{
public:
  Book(std::string const& title) : title{title} {}

  virtual ~Book() = default;
  virtual bool for_children() const = 0;
  std::string get_title() const
  {
    return title;
  }

  private:
  std::string const title;
};

class Non_Fiction : public Book
{
public:
  using Book::Book;
  bool for_children() const override
  {
    return false;
  }
};

class Fiction : public Book
{
public:
  Fiction(std::string const& title, bool for_kids) : Book{title}, for_kids{for_kids} {}
  Fiction(std::string const& title) : Book{title}, for_kids{false} {}
  virtual bool for_children() const override
  {
    return for_kids;
  }
private:
  bool for_kids;
};

class Crime_Fiction : public Fiction
{
public:
  Crime_Fiction(std::string title, bool for_kids) : Fiction{title + " (best seller)", for_kids} {}
  Crime_Fiction(std::string title) : Fiction{title + " (best seller)", false} {}
};

class Reader
{
public:
  Reader(std::string const& name) : name{name} {}

  virtual ~Reader() = default;
  virtual bool likes(Book const* book) const
  {
    return true;
  }
  void read(Book const* book) const
  {
    if (likes(book))
    {
      std::cout << name << " recommends reading " << book->get_title() << std::endl;
    }
    else
    {
      std::cout << name << " does not recommends reading "<< book->get_title() << std::endl;
    }
  }
private:
  std::string name;
};

class Academic : public Reader
{
public:
  using Reader::Reader;

  bool likes(Book const* book) const override
  {
    if (Non_Fiction const* non_fiction = dynamic_cast<Non_Fiction const*>(book); non_fiction != nullptr)
    {
      return true;
    }
    return false;
  }
};

class Child : public Reader
{
public:
  using Reader::Reader;

  bool likes(Book const* book) const override
  {
    return book->for_children();
  }

};

class Detective : public Reader
{
  using Reader::Reader;

  bool likes(Book const* book) const override
  {
    if (typeid(*book) == typeid(Fiction))
    {
      return true;
    }
    return false;
  }
};




/* if implemented correctly this program
   should give the following output:

Rhea Deer recommends reading "The Lord of the Rings".
Rhea Deer recommends reading "Winnie the Pooh".
Rhea Deer recommends reading "The Da Vinci Code (best seller)".
Rhea Deer recommends reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Rhea Deer recommends reading "Course Literature".
Professor McProfessorsen does not recommend reading "The Lord of the Rings".
Professor McProfessorsen does not recommend reading "Winnie the Pooh".
Professor McProfessorsen does not recommend reading "The Da Vinci Code (best seller)".
Professor McProfessorsen does not recommend reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Professor McProfessorsen recommends reading "Course Literature".
Christopher Robin does not recommend reading "The Lord of the Rings".
Christopher Robin recommends reading "Winnie the Pooh".
Christopher Robin does not recommend reading "The Da Vinci Code (best seller)".
Christopher Robin recommends reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Christopher Robin does not recommend reading "Course Literature".
Sherlock Holmes recommends reading "The Lord of the Rings".
Sherlock Holmes recommends reading "Winnie the Pooh".
Sherlock Holmes does not recommend reading "The Da Vinci Code (best seller)".
Sherlock Holmes does not recommend reading "Nancy Drew: The Secret of the Old Clock (best seller)".
Sherlock Holmes does not recommend reading "Course Literature".
*/

int main()
{
  vector<Reader*> readers {
    new Reader{"Rhea Deer"},
    new Academic{"Professor McProfessorsen"},
    new Child{"Christopher Robin"},
    new Detective{"Sherlock Holmes"}
  };

  vector<Book*> books {
    new Fiction{"The Lord of the Rings"},
    new Fiction{"Winnie the Pooh", true},
    new Crime_Fiction{"The Da Vinci Code"},
    new Crime_Fiction{"Nancy Drew: The Secret of the Old Clock", true},
    new Non_Fiction{"Course Literature"}
  };

  for (auto reader : readers)
  {
    for (auto book : books)
    {
      // reader.read(book);
      // or
      reader->read(book);
    }
    delete reader;
  }
  for (auto book : books)
  {
    delete book;
  }
}
