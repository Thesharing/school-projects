import java.util.ArrayList;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_BookCatalog {
    private ArrayList<_13211415_洪志龙_7_BookSpecification> books;

    _13211415_洪志龙_7_BookCatalog(){
        books = new ArrayList<_13211415_洪志龙_7_BookSpecification>();
    }

    void addBook(_13211415_洪志龙_7_BookSpecification bookSpecification){
        books.add(bookSpecification);
    }

    _13211415_洪志龙_7_BookSpecification getBookSpecification(String isdn) {
        for (_13211415_洪志龙_7_BookSpecification bookSpecification : books) {
            if (bookSpecification.getIsbn() == isdn) {
                return bookSpecification;
            }
        }
        return null;
    }

    ArrayList<_13211415_洪志龙_7_BookSpecification> getBooks(){
        return books;
    }
}
