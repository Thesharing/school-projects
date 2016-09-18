import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Vector;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_StrategiesUI extends JFrame implements ActionListener {
    private _13211415_洪志龙_7_Controller controller;
    private int addType = 0;
    private int selectedStrategyNumber = 0;
    private int strategyType = 0;
    private _13211415_洪志龙_7_BookType bookType = _13211415_洪志龙_7_BookType.OTHER;

    private JPanel mainPanel = new JPanel();
    private JDialog dialogAdd = new JDialog();
    private JDialog dialogModify = new JDialog();

    private ModelTable modelTable = new ModelTable();
    private JTable table = new JTable(modelTable);
    private JScrollPane scrollPane = new JScrollPane(table);

    private JButton buttonAddFlat = new JButton("添加绝对值优惠策略");
    private JButton buttonAddPerc = new JButton("添加百分比折扣策略");
    private JButton buttonAddComp = new JButton("添加组合策略");
    private JButton buttonModify = new JButton("修改策略");
    private JButton buttonDelete = new JButton("删除策略");

    private JTextField textFieldNumber = new JTextField();
    private JTextField textFieldDetail = new JTextField();
    private JTextField textFieldNumber1 = new JTextField();
    private JTextField textFieldDetail1 = new JTextField();

    private JButton buttonOk = new JButton("确定");
    private JButton buttonCancel = new JButton("取消");
    private JButton buttonOk1 = new JButton("确定");
    private JButton buttonCancel1 = new JButton("取消");

    private JLabel labelDetail = new JLabel("内容：");
    private JLabel labelDetail2 = new JLabel("内容：");

    private JComboBox comboBoxType = new JComboBox();

    _13211415_洪志龙_7_StrategiesUI(_13211415_洪志龙_7_Controller controller) {
        this.controller = controller;
        int screenHeight = java.awt.Toolkit.getDefaultToolkit().getScreenSize().height;
        int screenWidth = java.awt.Toolkit.getDefaultToolkit().getScreenSize().width;

        setTitle("策略管理");
        mainPanel.add(scrollPane);
        mainPanel.add(buttonAddFlat);
        mainPanel.add(buttonAddPerc);
        mainPanel.add(buttonAddComp);
        mainPanel.add(buttonModify);
        mainPanel.add(buttonDelete);

        comboBoxType.addItem("非教材类计算机图书");
        comboBoxType.addItem("教材类图书");
        comboBoxType.addItem("连环画类图书");
        comboBoxType.addItem("养生类图书");
        comboBoxType.addItem("其他");
        GridLayout grid = new GridLayout(4, 2);
        dialogAdd.setLayout(grid);
        dialogAdd.add(new JLabel("策略号："));
        dialogAdd.add(textFieldNumber);
        dialogAdd.add(new JLabel("书的类型："));
        dialogAdd.add(comboBoxType);
        dialogAdd.add(labelDetail);
        dialogAdd.add(textFieldDetail);
        dialogAdd.add(buttonOk);
        dialogAdd.add(buttonCancel);
        dialogAdd.setSize(400, 300);
        dialogAdd.setLocation((screenWidth - 400) / 2, (screenHeight - 300) / 2);
        dialogAdd.setModal(true);
        dialogAdd.pack();
        textFieldNumber.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent e) {
                int keyChar = e.getKeyChar();
                if (keyChar <= KeyEvent.VK_0 || keyChar >= KeyEvent.VK_9) {
                    e.consume();
                }
            }
        });

        GridLayout grid1 = new GridLayout(3, 2);
        dialogModify.setLayout(grid1);
        dialogModify.add(new JLabel("策略号"));
        dialogModify.add(textFieldNumber1);
        dialogModify.add(labelDetail2);
        dialogModify.add(textFieldDetail1);
        dialogModify.add(buttonOk1);
        dialogModify.add(buttonCancel1);
        dialogModify.setSize(400, 300);
        dialogModify.setLocation((screenWidth - 400) / 2, (screenHeight - 300) / 2);
        dialogModify.pack();
        dialogModify.setModal(true);
        textFieldNumber1.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent e) {
                int keyChar = e.getKeyChar();
                if (keyChar <= KeyEvent.VK_0 || keyChar >= KeyEvent.VK_9) {
                    e.consume();
                }
            }
        });

        buttonAddFlat.addActionListener(this);
        buttonAddPerc.addActionListener(this);
        buttonAddComp.addActionListener(this);
        buttonModify.addActionListener(this);
        buttonDelete.addActionListener(this);
        buttonOk.addActionListener(this);
        buttonOk1.addActionListener(this);
        buttonCancel.addActionListener(this);
        buttonCancel1.addActionListener(this);

        modelTable.updateUI();

        add(mainPanel);
        pack();
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        table.setPreferredScrollableViewportSize(new Dimension(400, 200));
        table.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        setSize(500, 350);
        setResizable(false);
        setLocation((screenWidth - 500) / 2, (screenHeight - 400) / 2);
    }

    public _13211415_洪志龙_7_BookType int2BookType(int number) {
        switch (number) {
            case 0:
                return _13211415_洪志龙_7_BookType.NONCOMPUTER;
            case 1:
                return _13211415_洪志龙_7_BookType.TEXTBOOK;
            case 2:
                return _13211415_洪志龙_7_BookType.COMIC;
            case 3:
                return _13211415_洪志龙_7_BookType.HEALTH;
            default:
                return _13211415_洪志龙_7_BookType.OTHER;
        }
    }

    class ModelTable extends DefaultTableModel {
        private Vector TableData;
        private Vector TableTitle;

        public ModelTable() {
            TableData = new Vector();
            TableTitle = new Vector();
            setDataVector(TableData, TableTitle);
            TableTitle.add("序号");
            TableTitle.add("类型");
            TableTitle.add("内容");
            TableTitle.add("图书类型");
        }

        public void addItem(_13211415_洪志龙_7_BookType bookType, _13211415_洪志龙_7_IPricingStrategy strategy) {
            ArrayList line = new ArrayList();
            line.add(strategy.getStrategyNumber());
            switch (strategy.getStrategyType()) {
                case 0:
                    line.add("绝对值优惠");
                    break;
                case 1:
                    line.add("百分比折扣");
                    break;
                case 2:
                    line.add("组合策略");
                    break;
                case 3:
                    line.add("无策略");
                    break;
                default:
                    line.add("错误");
            }
            line.add(strategy.toString());
            String bookTypeStr = "";
            switch (bookType) {
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
            TableData.add(line);
        }

        public int getSelectedStrategyNumber(int rowIndex) {
            ArrayList list = (ArrayList) this.TableData.get(rowIndex);
            return (int) list.get(0);
        }

        public void updateUI() {
            TableData.clear();
            for (HashMap.Entry<_13211415_洪志龙_7_BookType, _13211415_洪志龙_7_IPricingStrategy> entry : controller.getStrategyCatalog().getStrategies().entrySet()) {
                addItem(entry.getKey(), entry.getValue());
            }
            table.repaint();
            table.updateUI();
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
        if (e.getSource() == buttonAddFlat) {
            labelDetail.setText("每本书减：");
            dialogAdd.setTitle("添加绝对值优惠策略");
            textFieldDetail.setText("");
            textFieldNumber.setText("");
            addType = 1;
            dialogAdd.setVisible(true);
        }
        else if (e.getSource() == buttonAddPerc) {
            labelDetail.setText("百分比折扣：");
            dialogAdd.setTitle("添加百分比折扣策略");
            textFieldDetail.setText("");
            textFieldNumber.setText("");
            addType = 2;
            dialogAdd.setVisible(true);
        }
        else if (e.getSource() == buttonAddComp) {
            labelDetail.setText("按照1|2|...的格式来输入要组合的策略：");
            dialogAdd.setTitle("添加组合策略");
            textFieldNumber.setText("");
            textFieldDetail.setText("");
            addType = 3;
            dialogAdd.setVisible(true);
        }
        else if (e.getSource() == buttonDelete) {
            if (table.getSelectedRow() == -1) {
                JOptionPane.showMessageDialog(null, "请选中要删除的项。", "错误", JOptionPane.ERROR_MESSAGE);
            }
            else {
                selectedStrategyNumber = modelTable.getSelectedStrategyNumber(table.getSelectedRow());
                ArrayList<_13211415_洪志龙_7_IPricingStrategy> list = controller.findCompsiteContainSimple(selectedStrategyNumber);
                if (!list.isEmpty()) {
                    String strList = "";
                    for (_13211415_洪志龙_7_IPricingStrategy strategy : list) {
                        strList += String.valueOf(strategy.getStrategyNumber()) + " ";
                        controller.deleteStrategy(strategy.getStrategyNumber());
                    }
                    JOptionPane.showMessageDialog(null, "已删除" + strList + "策略，因为这些策略包含了要删除的策略。", "警告", JOptionPane.WARNING_MESSAGE);
                }
                controller.deleteStrategy(selectedStrategyNumber);
                modelTable.updateUI();
            }
        }
        else if (e.getSource() == buttonCancel) {
            textFieldNumber.setText("");
            textFieldDetail.setText("");
            dialogAdd.setVisible(false);
        }
        else if (e.getSource() == buttonCancel1) {
            textFieldDetail1.setText("");
            textFieldNumber1.setText("");
            dialogModify.setVisible(false);
        }
        else if (e.getSource() == buttonModify) {
            if (table.getSelectedRow() == -1) {
                JOptionPane.showMessageDialog(null, "请选中要修改的项。", "错误", JOptionPane.ERROR_MESSAGE);
            }
            else {
                textFieldNumber1.setText("");
                textFieldDetail1.setText("");
                selectedStrategyNumber = modelTable.getSelectedStrategyNumber(table.getSelectedRow());
                strategyType = controller.getStrategyType(selectedStrategyNumber);
                textFieldNumber1.setText(String.valueOf(selectedStrategyNumber));
                bookType = controller.getStrategyCatalog().getKeyFromNumber(selectedStrategyNumber);
                dialogModify.setVisible(true);
            }
        }
        else if (e.getSource() == buttonOk) {
            if (textFieldNumber.getText().isEmpty() || textFieldDetail.getText().isEmpty()) {
                JOptionPane.showMessageDialog(null, "请填写全部信息。", "错误", JOptionPane.ERROR_MESSAGE);
            }
            else {
                int number;
                try {
                    number = Integer.parseInt(textFieldNumber.getText());
                }
                catch (java.lang.NumberFormatException e1) {
                    JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                    return;
                }
                switch (addType) {
                    case 1:
                        try {
                            Integer.parseInt(textFieldDetail.getText());
                        }
                        catch (java.lang.NumberFormatException e1) {
                            JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.addStrategy(0, number, int2BookType(comboBoxType.getSelectedIndex()), textFieldDetail.getText())) {
                            JOptionPane.showMessageDialog(null, "添加成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail.setText("");
                            textFieldNumber.setText("");
                            dialogAdd.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "添加失败,策略号和图书类型均不能重复。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    case 2:
                        double percentage;
                        try {
                            percentage = Float.parseFloat(textFieldDetail.getText());
                        }
                        catch (java.lang.NumberFormatException e1) {
                            JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (percentage > 100) {
                            JOptionPane.showMessageDialog(null, "折扣不得大于100%。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.addStrategy(1, number, int2BookType(comboBoxType.getSelectedIndex()), textFieldDetail.getText())) {
                            JOptionPane.showMessageDialog(null, "添加成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail.setText("");
                            textFieldNumber.setText("");
                            dialogAdd.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "添加失败,策略号和图书类型均不能重复。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    case 3:
                        if (controller.addStrategy(2, number, int2BookType(comboBoxType.getSelectedIndex()), textFieldDetail.getText())) {
                            JOptionPane.showMessageDialog(null, "添加成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail.setText("");
                            textFieldNumber.setText("");
                            dialogAdd.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "添加失败,策略号和图书类型均不能重复，不能包含复合策略。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    default:
                        JOptionPane.showMessageDialog(null, "添加失败,请检查填写的信息是否正确。", "提示", JOptionPane.ERROR_MESSAGE);
                        return;
                }
                modelTable.updateUI();
                textFieldNumber.setText("");
                textFieldDetail.setText("");
            }
        }
        else if (e.getSource() == buttonOk1) {
            if (textFieldNumber1.getText().isEmpty() || textFieldDetail1.getText().isEmpty()) {
                JOptionPane.showMessageDialog(null, "请填写全部信息。", "错误", JOptionPane.ERROR_MESSAGE);
                return;
            }
            else {
                int number;
                try {
                    number = Integer.parseInt(textFieldNumber1.getText());
                }
                catch (java.lang.NumberFormatException e1) {
                    JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                    return;
                }
                switch (strategyType) {
                    case 0:
                        try {
                            Integer.parseInt(textFieldDetail1.getText());
                        }
                        catch (java.lang.NumberFormatException e1) {
                            JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.updateStrategy(0, number, bookType, textFieldDetail1.getText())) {
                            JOptionPane.showMessageDialog(null, "修改成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail1.setText("");
                            textFieldNumber1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "修改失败,策略号和图书类型均不能重复。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    case 1:
                        double perscentage;
                        try {
                            perscentage = Float.parseFloat(textFieldDetail1.getText());
                        }
                        catch (java.lang.NumberFormatException e1) {
                            JOptionPane.showMessageDialog(null, "请填写数字。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (perscentage > 100) {
                            JOptionPane.showMessageDialog(null, "折扣不能大于100%。", "错误", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        if (controller.updateStrategy(1, number, bookType, textFieldDetail1.getText())) {
                            JOptionPane.showMessageDialog(null, "修改成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail1.setText("");
                            textFieldNumber1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "修改失败,策略号和图书类型均不能重复。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    case 2:
                        if (controller.updateStrategy(2, number, bookType, textFieldDetail1.getText())) {
                            JOptionPane.showMessageDialog(null, "修改成功。", "提示", JOptionPane.INFORMATION_MESSAGE);
                            textFieldDetail1.setText("");
                            textFieldNumber1.setText("");
                            dialogModify.setVisible(false);
                        }
                        else {
                            JOptionPane.showMessageDialog(null, "修改失败,策略号和图书类型均不能重复，不能包含复合类型。", "提示", JOptionPane.ERROR_MESSAGE);
                            return;
                        }
                        break;
                    default:
                        JOptionPane.showMessageDialog(null, "修改失败。", "错误", JOptionPane.ERROR_MESSAGE);
                        return;
                }
                modelTable.updateUI();
                textFieldNumber1.setText("");
                textFieldDetail1.setText("");
            }
        }
    }
}
