import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_AddBookUI extends JFrame implements ActionListener {
    private _13211415_洪志龙_7_Controller controller;

    private JTextField textFieldPrice = new JTextField();
    private JTextField textFieldTitle = new JTextField();
    private JTextField textFieldISDN = new JTextField();
    private JComboBox comboBoxType = new JComboBox();
    private JButton buttonOk = new JButton("确定");
    private JButton buttonClear = new JButton("清空");

    _13211415_洪志龙_7_AddBookUI(_13211415_洪志龙_7_Controller controller){
        this.controller=controller;

        GridLayout grid=new GridLayout(5,2);
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(grid);
        mainPanel.add(new JLabel("书名:"));
        mainPanel.add(textFieldTitle);
        mainPanel.add(new JLabel("ISBN："));
        mainPanel.add(textFieldISDN);
        mainPanel.add(new JLabel("价格："));
        mainPanel.add(textFieldPrice);
        mainPanel.add(new JLabel("类型："));
        mainPanel.add(comboBoxType);
        mainPanel.add(buttonOk);
        mainPanel.add(buttonClear);
        comboBoxType.addItem("非教材类计算机图书");
        comboBoxType.addItem("教材类图书");
        comboBoxType.addItem("连环画类图书");
        comboBoxType.addItem("养生类图书");
        comboBoxType.addItem("其他");
        pack();

        textFieldPrice.addKeyListener(new KeyAdapter(){
            public void keyTyped(KeyEvent e) {
                int keyChar = e.getKeyChar();
                if(keyChar >= KeyEvent.VK_0 && keyChar <= KeyEvent.VK_9 || keyChar == KeyEvent.VK_PERIOD){
                }
                else{
                    e.consume();
                }
            }
        });

        textFieldISDN.addKeyListener(new KeyAdapter(){
            public void keyTyped(KeyEvent e) {
                int keyChar = e.getKeyChar();
                if(keyChar >= KeyEvent.VK_0 && keyChar <= KeyEvent.VK_9 || keyChar == KeyEvent.VK_MINUS){
                }
                else{
                    e.consume();
                }
            }
        });

        buttonOk.addActionListener(this);
        buttonClear.addActionListener(this);

        add(mainPanel);
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        int screenHeight = java.awt.Toolkit.getDefaultToolkit().getScreenSize().height;
        int screenWidth = java.awt.Toolkit.getDefaultToolkit().getScreenSize().width;
        setTitle("添加书籍信息");
        setResizable(false);
        setSize(300, 200);
        setLocation((screenWidth - 300) / 2, (screenHeight - 200) / 2);
        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getSource()==buttonOk){
            if(textFieldPrice.getText().isEmpty() || textFieldISDN.getText().isEmpty() ||
                    textFieldTitle.getText().isEmpty()){
                JOptionPane.showMessageDialog(null,"请填写全部信息。","错误",JOptionPane.ERROR_MESSAGE);
            }
            else{
                float price = 0;
                try{
                    price = Float.parseFloat(textFieldPrice.getText());
                }
                catch(java.lang.NumberFormatException e1){
                    JOptionPane.showMessageDialog(null,"价格输入格式错误。","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
                String title = textFieldTitle.getText();
                String isbn = textFieldISDN.getText();
                _13211415_洪志龙_7_BookType bookType;
                switch (comboBoxType.getSelectedIndex()){
                    case 0:bookType= _13211415_洪志龙_7_BookType.NONCOMPUTER;
                        break;
                    case 1:bookType= _13211415_洪志龙_7_BookType.TEXTBOOK;
                        break;
                    case 2:bookType= _13211415_洪志龙_7_BookType.COMIC;
                        break;
                    case 3:bookType= _13211415_洪志龙_7_BookType.HEALTH;
                        break;
                    case 4:bookType= _13211415_洪志龙_7_BookType.OTHER;
                        break;
                    default:
                        bookType= _13211415_洪志龙_7_BookType.NOTEXIST;
                        break;
                }
                System.out.println(bookType.toString());
                controller.addBook(new _13211415_洪志龙_7_BookSpecification(isbn,price,title,bookType));
                setVisible(false);
            }
        }
        else if(e.getSource()==buttonClear){
            textFieldISDN.setText("");
            textFieldTitle.setText("");
            textFieldPrice.setText("");
        }
    }
}