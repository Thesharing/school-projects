# 《机器学习》第二次作业

### 11.3 写出条件随机场模型学习的梯度下降法

条件随机场模型可表示为：

$$
P\left( y \mid x\right) =\dfrac {1}{Z\left( x\right) }\exp \sum ^{K}_{k=1}w_{k}f_{k}\left( y,x\right) \tag{1}
$$

$$
Z\left( x \right) = \sum_{y}\exp \sum^{K}_{k=1}w_kf_k\left( y, x \right) \tag{2}
$$

由教材可知，可以通过极大化训练数据的对数似然函数来求模型参数，对于条件随机场模型来说，其对数似然函数为：

$$
\begin{align}
L\left( w \right) & = \sum_{j=1}^{N}\sum_{k=1}^{K}w_{k}f_{k}\left(y_j,x_j\right) - \sum_{j=1}^N\log{Z_w\left(x_{j}\right)} \\
& = \sum_{j=1}^{N}\left(\sum_{k=1}^{K}w_{k}f_{k}\left(y_{j}, x_{j}\right) - \log{Z_w\left(x_j\right)}\right) \tag{3}
\end{align}
$$

我们先只考虑括号里的内容，设$l(w)=\sum_{k=1}^{K}w_{k}f_{k}\left(y, x\right)-\log{Z_{w}\left(x\right)}$，然后求$l\left(w\right)$关于$w_k$的偏导，即

$$
\frac{\partial{\,l\left(w\right)}}{\partial \,w_{k}} =f_k(y, x)-\frac{\partial{\,\log{Z_{w}\left(x\right)}}}{\partial{\,w_{k}}} \tag{4}
$$

接下来重点求后半部分：

$$
\begin{align}
\frac{\partial{\,\log{Z_{w}\left(x\right)}}}{\partial{\,w_{k}}} & = \frac{1}{Z_{w}\left(x\right)}\cdot\frac{\partial{\,Z_{w}\left(x\right)}}{\partial{\,w_{k}}} \\
& =  \frac{1}{Z_{w}\left(x\right)}\cdot\frac{\partial{\,\sum_{y}\exp{\left(\sum_{k=1}^{K}w_{k}f_k\left(y,x\right)\right)}}}{\partial{\,w_k}} \\
& = \frac{1}{Z_{w}\left(x\right)} \cdot \sum_{y}\left(\exp{\left(\sum_{k=1}^{K}w_{k}f_k\left(y,x\right)\right)} \cdot f_k\left(y,x\right)\right) \tag{5}
\end{align}
$$

由于$Z_w\left(x\right)$在式中为常数，因此将$Z_w\left(x\right)$带入 $\sum$式中，得到

$$
\begin{align}
\frac{\partial{\,\log{Z_{w}\left(x\right)}}}{\partial{\,w_{k}}} & = \sum_{y}\left(\frac{{\exp\left(\sum_{k=1}^{K}w_{k}f_k\left(y,x\right)\right)}}{ {Z_{w}\left(x\right)}} \cdot f_k\left(y,x\right)\right) \\
& = \sum_y\left(P\left(y \mid x\right) \cdot f_k\left(y,x\right)\right) \tag{6}
\end{align}
$$

然后将 $(6)​$ 式带回到 $(4)​$ 式中，得到

$$
\frac{\partial{\,l\left(w\right)}}{\partial{\,w_k} } =  f_k(y, x) - \sum_y\left(P\left(y \mid x\right) \cdot f_k\left(y,x\right)\right) \tag{7}
$$

接下来根据 $(3)$ 式和 $(7)$ 式我们可以得到

$$
\begin{align}
g_k &= g\left(w^{\left(k\right)}\right)\\
&= \nabla L\left(w_k\right) \\
&=\sum_{j=1}^N \left(f_k(y_j, x_j) - \sum_y\left(P\left(y \mid x_j\right) \cdot f_k\left(y,x_j\right)\right) \right) \\
&=\sum_{j=1}^N \left(f_k(y_j, x_j) - E_{P(Y \mid X)}{[}f_k]\right) \tag{8}\\
\end{align}
$$

式中

$$
\begin{align}
f_k(y,x)&=\sum_{i=1}^n f_k\left(y_{i-1}, y_i, x, i\right) \text{,  $ k=1, 2, \cdots,K$ } \\
E_{P(Y \mid X)}[f_k]&=\sum_yP(y \mid x)f_k(y,x) \\
&=\sum_{i=1}^{n+1}\sum_{y_{i-1}y_i}f_k\left(y_{i-1}, y_i, x, i\right)\frac{\alpha^T_{i-1}\left(y_{i-1}\mid x\right)M_i(y_{i-1}, y_i \mid x)\beta_i(y_i\mid x)}{Z(x)} \tag{9}
\end{align}
\\ k=1, 2, \cdots, K
$$

即通过前向-后向向量来计算。

因此梯度下降法的算法如下所示：

-----

**算法 2.1 梯度下降法**

输入：特征函数$f_1, f_2, \cdots, f_n$；经验分布$\tilde{P}(X, Y)$，计算精度$\varepsilon$；

输出：$L(w)$的极小点$w^*$

(1) 取初始值$x^{(0)}\in \mathbb{R}_n$，置$k=0$

(2) 计算$L\left(w^{\left(k\right)}\right)$

(3) 按照式 $(8)$ 计算梯度$g_k=g\left(w^{\left(k\right)}\right)$，当$\Vert g_k\Vert < \varepsilon$时，停止迭代，令$w^* = w^{\left(k\right)}$；否则，令$p_k=-g\left(w^{\left(k\right)}\right)$，求$\lambda_k$，使得

$$
L\left(w^{\left(k\right)}+\lambda_kp_k\right) = \min\limits_{\lambda\geq 0}f\left(w^{(k)}+\lambda p_k\right)
$$

(4) 置$w^{\left(k+1\right)}=w^{(k)} + \lambda_k p_k$，计算$f\left(w^{\left(k + 1\right)}\right)$

当$\Vert f\left(w^{\left(k+1\right)} \right) - f\left(x^{\left(k\right)}\right)\Vert < \varepsilon$或$\Vert w^{\left(k+1\right)}-w^{\left(k\right)}\Vert<\varepsilon$时，停止迭代，令$w^*=x^{(k+1)}$

(5) 否则，置$k=k+1$，转 (3)