/**
 * Created by Hongzhilong on 15/12/21.
 */

import java.lang.String;

        public class _13211415_洪志龙_7_BookSpecification {
    private String isbn;
    private double price;
    private String title;
    private _13211415_洪志龙_7_BookType type;

    public _13211415_洪志龙_7_BookSpecification(String isbn, double price, String title, _13211415_洪志龙_7_BookType type){
        this.isbn=isbn;
        this.price=price;
        this.title=title;
        this.type=type;
    }

    public _13211415_洪志龙_7_BookType getType(){return this.type;}

    public String getTitle(){return this.title;}

    public String getIsbn(){return this.isbn;}

    public double getPrice() {return this.price;}
}
