import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Vector;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_MainUI extends JFrame implements ActionListener,_13211415_洪志龙_7_ObserverForMainUI {
    private _13211415_洪志龙_7_Controller controller;

    private JPanel mainPanel = new JPanel();
    private ModelTable modelTable = new ModelTable();
    private JTable table = new JTable(modelTable);
    private JScrollPane scrollPane = new JScrollPane(table);
    private JButton buttonAddBook = new JButton("添加图书信息");
    private JButton buttonStrategies = new JButton("管理折扣策略");
    private JButton buttonBuy = new JButton("购买选中图书");

    _13211415_洪志龙_7_MainUI() {
        setTitle("图书销售系统");
        mainPanel.add(scrollPane);
        mainPanel.add(buttonAddBook);
        mainPanel.add(buttonStrategies);
        mainPanel.add(buttonBuy);

        controller = new _13211415_洪志龙_7_Controller();
        controller.registerObserver(this);
        if(JOptionPane.showConfirmDialog(null, "请问是否要自动添加信息呢？", "图书销售系统", JOptionPane.YES_NO_OPTION)==JOptionPane.YES_OPTION)
            initialize();

        buttonBuy.addActionListener(this);
        buttonAddBook.addActionListener(this);
        buttonStrategies.addActionListener(this);

        _13211415_洪志龙_7_ShoppingCartUI shoppingCartUI = new _13211415_洪志龙_7_ShoppingCartUI(controller.getSale());

        add(mainPanel);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        table.setPreferredScrollableViewportSize(new Dimension(400, 300));
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        int screenHeight = java.awt.Toolkit.getDefaultToolkit().getScreenSize().height;
        int screenWidth = java.awt.Toolkit.getDefaultToolkit().getScreenSize().width;
        setResizable(false);
        setSize(500, 400);
        setLocation((screenWidth - 100) / 2, (screenHeight - 400) / 2);
        setVisible(true);
    }

    class ModelTable extends DefaultTableModel {
        private Vector TableData;
        private Vector TableTitle;

        public ModelTable() {
            TableData = new Vector();
            TableTitle = new Vector();
            setDataVector(TableData, TableTitle);
            TableTitle.add("书名");
            TableTitle.add("ISBN");
            TableTitle.add("类型");
            TableTitle.add("价格");
        }

        public void addBook(_13211415_洪志龙_7_BookSpecification book) {
            ArrayList line = new ArrayList();
            line.add(book.getTitle());
            line.add(book.getIsbn());
            String bookTypeStr ="";
            switch (book.getType()){
                case NONCOMPUTER:
                    bookTypeStr = "非教材类计算机图书";
                    break;
                case TEXTBOOK:
                    bookTypeStr = "教材类图书";
                    break;
                case HEALTH:
                    bookTypeStr = "养生类图书";
                    break;
                case COMIC:
                    bookTypeStr = "连环画类图书";
                    break;
                case OTHER:
                    bookTypeStr = "其他类图书";
                    break;
            }
            line.add(bookTypeStr);
            line.add(book.getPrice());
            TableData.add(line);
            table.repaint();
            table.updateUI();
        }

        public String getISDN(int rowIndex) {
            ArrayList list = (ArrayList) this.TableData.get(rowIndex);
            return (String) list.get(1);
        }

        public void updateUI() {
            TableData.clear();
            for (_13211415_洪志龙_7_BookSpecification bookSpecification : controller.getBookCatalog().getBooks()) {
                addBook(bookSpecification);
            }
        }

        public int getColumnCount() {
            return TableTitle.size();
        }

        public Object getValueAt(int rowIndex, int columnIndex) {
            ArrayList list = (ArrayList) this.TableData.get(rowIndex);
            return list.get(columnIndex);
        }

        public boolean isCellEditable(int rowIndex, int columnIndex) {
            return false;
        }
    }


    public void actionPerformed(ActionEvent e) {
        if(e.getSource()==buttonAddBook){
            _13211415_洪志龙_7_AddBookUI addBookUI = new _13211415_洪志龙_7_AddBookUI(controller);
            addBookUI.setVisible(true);
        }
        else if(e.getSource()==buttonBuy){
            if (table.getSelectedRow() == -1) {
                JOptionPane.showMessageDialog(null, "请选中要购买的书籍。", "错误", JOptionPane.ERROR_MESSAGE);
            } else {
                _13211415_洪志龙_7_BuyUI buyUI = new _13211415_洪志龙_7_BuyUI(controller,modelTable.getISDN(table.getSelectedRow()));
                buyUI.setVisible(true);
            }
        }
        else if(e.getSource()==buttonStrategies){
            _13211415_洪志龙_7_StrategiesUI strategiesUI = new _13211415_洪志龙_7_StrategiesUI(controller);
            strategiesUI.setVisible(true);
        }
    }

    public void update(){
        modelTable.updateUI();
    }

    public void initialize(){
        controller.addBook(new _13211415_洪志龙_7_BookSpecification("978-7-302-2", 18, "UML与模式应用", _13211415_洪志龙_7_BookType.TEXTBOOK));
        controller.addBook(new _13211415_洪志龙_7_BookSpecification("978-7-312-3", 34, "Java与模式", _13211415_洪志龙_7_BookType.NONCOMPUTER));
        controller.addBook(new _13211415_洪志龙_7_BookSpecification("986-6-302-1", 58, "Head First设计模式", _13211415_洪志龙_7_BookType.NONCOMPUTER));
        controller.addBook(new _13211415_洪志龙_7_BookSpecification("958-1-302-2", 30, "爱丽丝历险记", _13211415_洪志龙_7_BookType.COMIC));
        controller.addBook(new _13211415_洪志龙_7_BookSpecification("900-7-392-2", 20, "煲汤大全", _13211415_洪志龙_7_BookType.HEALTH));
        controller.addStrategy(0,1, _13211415_洪志龙_7_BookType.TEXTBOOK,String.valueOf(1));
        controller.addStrategy(1,2, _13211415_洪志龙_7_BookType.COMIC,String.valueOf(7));
        controller.addStrategy(1,3, _13211415_洪志龙_7_BookType.NONCOMPUTER,String.valueOf(3));
        controller.addStrategy(2,4, _13211415_洪志龙_7_BookType.HEALTH,"1|3");
    }
}
