# 利用python实现数据库连接
import pyodbc

con = pyodbc.connect(driver='{ODBC Driver 17 for SQL Server}', server='tcp:localhost,1433',
                     database='shujuku1', uid='sa', pwd='123456')
if con:
    print('连接成功')
curso = con.cursor()
# 删除表
try:
    curso.execute("DROP TABLE shu_xuje")
except:
    print('表名不存在')

# 创建一个名maoyandb的表 并且添加字段zhanghu passed
curso.execute("CREATE TABLE shu_xuje (zhanghu VARCHAR(50), passed VARCHAR(10))")

# 写入zhanghu passed
st1 = input('请输入创建的账户：》》')
st2 = input('请输入密码:>>')
curso.execute("INSERT INTO shu_xuje (zhanghu, passed) VALUES(?,?)", st1, st2)
curso.execute("INSERT INTO shu_xuje (zhanghu, passed) VALUES(?,?)", "sfs", "fse")

# 插入记录
st1 = input('请输入创建的账户：》》')
st2 = input('请输入密码:>>')
curso.execute("INSERT INTO shu_xuje VALUES(?,?)", st1, st2)

# 查询zhanhu passed
st1 = input('请输入查询的账户名字:>>')
curso.execute("SELECT * FROM shu_xuje WHERE zhanghu=?", st1)

# 获取查询的结果  [('zhsuf', 'sdeess')]
row = curso.fetchall()
if row:
    print('查询结果:>>', row)
else:
    print('查询无结果')

# 登录
while True:
    st1 = input('请输入登录的账户:>>')
    str2 = input('请输入登录的密码:>>')
    curso.execute("SELECT * FROM shu_xuje WHERE zhanghu=?", st1)
    row = curso.fetchall()
    if row:
        print('登录成功！')
        break
    else:
        print('无此账户或者密码不正确，请重新输入')

# 删除字段
st1 = input('请输入你想删除的账户:>>')
curso.execute("SELECT * FROM shu_xuje WHERE zhanghu=?", st1)
row = curso.fetchall()
if row:
    curso.execute("DELETE FROM shu_xuje WHERE zhanghu=?", st1)
    print("删除成功")
else:
    print('无此账户')

con.commit()
curso.close()
con.close()
