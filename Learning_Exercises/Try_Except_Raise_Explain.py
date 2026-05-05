"""
Python中raise语句的完整示例
包含：基本用法、自定义异常、异常链、最佳实践等
"""

print("=" * 60)
print("Python raise语句综合示例")
print("=" * 60)

# ============================================================================
# 1. 基础异常抛出示例
# ============================================================================
print("\n1. 基础异常抛出示例")
print("-" * 40)

def basic_raise_example():
    """基本的raise用法"""
    print("基本raise用法演示：")
    
    # 示例1: 抛出带消息的异常
    try:
        age = -5
        if age < 0:
            raise ValueError(f"年龄不能为负数: {age}")
    except ValueError as e:
        print(f"  捕获到ValueError: {e}")
    
    # 示例2: 重新抛出异常
    try:
        try:
            result = 10 / 0
        except ZeroDivisionError:
            print("  内部: 捕获到除零错误，重新抛出")
            raise  # 重新抛出当前异常
    except ZeroDivisionError:
        print("  外部: 捕获到重新抛出的除零错误")
    
    # 示例3: 不同的异常类型
    try:
        data = {"name": "Alice"}
        key = "age"
        if key not in data:
            raise KeyError(f"字典中不存在键: {key}")
    except KeyError as e:
        print(f"  捕获到KeyError: {e}")

basic_raise_example()

# ============================================================================
# 2. 自定义异常类
# ============================================================================
print("\n2. 自定义异常类")
print("-" * 40)

class ValidationError(Exception):
    """基础验证异常"""
    pass

class EmailFormatError(ValidationError):
    """邮箱格式错误"""
    def __init__(self, email):
        self.email = email
        super().__init__(f"邮箱格式错误: {email}")

class PasswordStrengthError(ValidationError):
    """密码强度不足"""
    def __init__(self, reason):
        self.reason = reason
        super().__init__(f"密码强度不足: {reason}")

class UserRegistrationError(Exception):
    """用户注册异常"""
    pass

def validate_user_data(email, password):
    """验证用户数据"""
    print(f"验证用户数据: email={email}, password={password}")
    
    # 验证邮箱格式
    if "@" not in email or "." not in email.split("@")[-1]:
        raise EmailFormatError(email)
    
    # 验证密码强度
    if len(password) < 8:
        raise PasswordStrengthError("密码长度至少8位")
    if not any(c.isdigit() for c in password):
        raise PasswordStrengthError("密码必须包含数字")
    if not any(c.isalpha() for c in password):
        raise PasswordStrengthError("密码必须包含字母")
    
    print("  验证通过!")
    return True

def register_user(email, password):
    """注册用户"""
    try:
        validate_user_data(email, password)
        # 模拟注册逻辑
        print(f"  用户 {email} 注册成功!")
        return True
    except ValidationError as e:
        # 包装验证异常
        raise UserRegistrationError(f"注册失败: {e}") from e

# 测试自定义异常
try:
    register_user("test@example.com", "weak")
except UserRegistrationError as e:
    print(f"注册失败: {e}")
    if e.__cause__:
        print(f"  原因: {e.__cause__}")

try:
    register_user("test@example.com", "StrongPass123")
except UserRegistrationError as e:
    print(f"注册失败: {e}")

# ============================================================================
# 3. 异常链示例
# ============================================================================
print("\n3. 异常链示例")
print("-" * 40)

def read_config_file(filename):
    """读取配置文件"""
    try:
        with open(filename, 'r') as f:
            content = f.read()
        # 模拟解析JSON
        if not content.startswith("{"):
            raise ValueError("不是有效的JSON格式")
        return content
    except FileNotFoundError as e:
        # 使用异常链
        raise RuntimeError(f"无法读取配置文件: {filename}") from e
    except (ValueError, UnicodeDecodeError) as e:
        raise RuntimeError(f"配置文件格式错误: {filename}") from e

try:
    config = read_config_file("nonexistent_config.json")
except RuntimeError as e:
    print(f"运行时错误: {e}")
    if e.__cause__:
        print(f"  原始异常: {type(e.__cause__).__name__}: {e.__cause__}")

# ============================================================================
# 4. 上下文管理器中的异常处理
# ============================================================================
print("\n4. 上下文管理器中的异常处理")
print("-" * 40)

class DatabaseConnection:
    """模拟数据库连接"""
    
    def __init__(self, db_name):
        self.db_name = db_name
        self.connected = False
    
    def __enter__(self):
        """进入上下文"""
        print(f"  正在连接数据库: {self.db_name}")
        # 模拟连接失败
        if "invalid" in self.db_name:
            raise ConnectionError(f"无法连接到数据库: {self.db_name}")
        self.connected = True
        print("  数据库连接成功!")
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """退出上下文"""
        if self.connected:
            print("  关闭数据库连接")
            self.connected = False
        
        # 如果发生了异常，可以在这里处理或重新抛出
        if exc_type is not None:
            print(f"  在数据库上下文中发生异常: {exc_type.__name__}: {exc_val}")
            # 返回True表示异常已处理，不再传播
            # 返回False或None表示异常继续传播
            return False  # 让异常继续传播
    
    def execute_query(self, query):
        """执行查询"""
        if not self.connected:
            raise RuntimeError("数据库未连接")
        
        if "DROP" in query.upper():
            raise ValueError("不允许执行DROP语句")
        
        print(f"  执行查询: {query}")
        return "查询结果"

# 测试上下文管理器
print("测试有效的数据库连接:")
try:
    with DatabaseConnection("my_database") as db:
        result = db.execute_query("SELECT * FROM users")
        print(f"  结果: {result}")
except Exception as e:
    print(f"  发生异常: {e}")

print("\n测试无效的数据库连接:")
try:
    with DatabaseConnection("invalid_db") as db:
        result = db.execute_query("SELECT * FROM users")
except Exception as e:
    print(f"  发生异常: {e}")

print("\n测试危险查询:")
try:
    with DatabaseConnection("my_database") as db:
        result = db.execute_query("DROP TABLE users")
except Exception as e:
    print(f"  发生异常: {e}")

# ============================================================================
# 5. 装饰器中的异常处理
# ============================================================================
print("\n5. 装饰器中的异常处理")
print("-" * 40)

def retry_on_exception(max_retries=3, delay=1):
    """异常重试装饰器"""
    import time
    
    def decorator(func):
        def wrapper(*args, **kwargs):
            for attempt in range(max_retries):
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    if attempt == max_retries - 1:
                        # 最后一次尝试也失败了，抛出异常
                        raise RuntimeError(f"函数 {func.__name__} 在 {max_retries} 次重试后仍然失败") from e
                    
                    print(f"  尝试 {func.__name__} 失败 (第{attempt+1}次): {e}")
                    print(f"  {delay}秒后重试...")
                    time.sleep(delay)
            return None
        return wrapper
    return decorator

def validate_input_types(**type_hints):
    """输入类型验证装饰器"""
    def decorator(func):
        def wrapper(*args, **kwargs):
            # 将args转换为字典形式，简化处理
            func_args = func.__code__.co_varnames[:func.__code__.co_argcount]
            
            # 检查位置参数
            for i, (arg_name, expected_type) in enumerate(type_hints.items()):
                if i < len(args):
                    value = args[i]
                    if not isinstance(value, expected_type):
                        raise TypeError(
                            f"参数 '{arg_name}' 应该是 {expected_type.__name__} 类型, "
                            f"但接收到 {type(value).__name__}: {value}"
                        )
            
            # 检查关键字参数
            for arg_name, expected_type in type_hints.items():
                if arg_name in kwargs:
                    value = kwargs[arg_name]
                    if not isinstance(value, expected_type):
                        raise TypeError(
                            f"参数 '{arg_name}' 应该是 {expected_type.__name__} 类型, "
                            f"但接收到 {type(value).__name__}: {value}"
                        )
            
            return func(*args, **kwargs)
        return wrapper
    return decorator

# 使用装饰器的示例
@retry_on_exception(max_retries=2, delay=0.5)
@validate_input_types(x=int, y=int)
def divide_numbers(x, y):
    """除法函数，模拟可能失败的操作"""
    if y == 0:
        raise ValueError("除数不能为零")
    
    # 模拟随机失败
    import random
    if random.random() < 0.5:  # 50%概率失败
        raise RuntimeError("随机操作失败")
    
    return x / y

print("测试带装饰器的函数:")
for i in range(3):
    print(f"\n尝试 {i+1}:")
    try:
        result = divide_numbers(10, 2)
        print(f"  结果: {result}")
    except Exception as e:
        print(f"  最终失败: {e}")
        if e.__cause__:
            print(f"    原因: {e.__cause__}")

# 测试类型验证
print("\n测试类型验证装饰器:")
try:
    result = divide_numbers(10, "2")  # 错误类型
except Exception as e:
    print(f"  类型错误: {e}")

# ============================================================================
# 6. 完整的业务逻辑示例
# ============================================================================
print("\n6. 完整的业务逻辑示例")
print("-" * 40)

class BankAccount:
    """银行账户类"""
    
    def __init__(self, account_holder, initial_balance=0):
        if not isinstance(account_holder, str) or len(account_holder.strip()) == 0:
            raise ValueError("账户持有人姓名不能为空")
        
        if initial_balance < 0:
            raise ValueError("初始余额不能为负数")
        
        self.account_holder = account_holder
        self._balance = initial_balance
        self.transaction_history = []
    
    @property
    def balance(self):
        return self._balance
    
    def deposit(self, amount):
        """存款"""
        if amount <= 0:
            raise ValueError(f"存款金额必须为正数: {amount}")
        
        self._balance += amount
        self.transaction_history.append(("存款", amount, self._balance))
        print(f"  存款成功: +${amount:.2f}, 余额: ${self._balance:.2f}")
        return self._balance
    
    def withdraw(self, amount):
        """取款"""
        if amount <= 0:
            raise ValueError(f"取款金额必须为正数: {amount}")
        
        if amount > self._balance:
            raise InsufficientFundsError(self._balance, amount)
        
        self._balance -= amount
        self.transaction_history.append(("取款", amount, self._balance))
        print(f"  取款成功: -${amount:.2f}, 余额: ${self._balance:.2f}")
        return self._balance
    
    def transfer(self, amount, to_account):
        """转账"""
        if not isinstance(to_account, BankAccount):
            raise TypeError(f"目标账户必须是BankAccount类型")
        
        if amount <= 0:
            raise ValueError(f"转账金额必须为正数: {amount}")
        
        print(f"  开始转账: ${amount:.2f} 从 {self.account_holder} 到 {to_account.account_holder}")
        
        # 使用事务性操作
        try:
            self.withdraw(amount)
            to_account.deposit(amount)
            self.transaction_history.append(("转账", amount, self._balance, to_account.account_holder))
            print(f"  转账成功!")
            return True
        except Exception as e:
            # 如果转账失败，记录日志但重新抛出异常
            print(f"  转账失败: {e}")
            raise
    
    def get_statement(self):
        """获取账户对账单"""
        statement = f"\n账户对账单 - {self.account_holder}\n"
        statement += "=" * 40 + "\n"
        statement += f"当前余额: ${self._balance:.2f}\n"
        statement += "-" * 40 + "\n"
        statement += "交易历史:\n"
        
        for transaction in self.transaction_history:
            if transaction[0] == "转账":
                statement += f"  {transaction[0]}: ${transaction[1]:.2f} 给 {transaction[3]}, 余额: ${transaction[2]:.2f}\n"
            else:
                statement += f"  {transaction[0]}: ${transaction[1]:.2f}, 余额: ${transaction[2]:.2f}\n"
        
        return statement

class InsufficientFundsError(Exception):
    """余额不足异常"""
    def __init__(self, balance, amount):
        self.balance = balance
        self.amount = amount
        shortfall = amount - balance
        super().__init__(
            f"余额不足! 当前余额: ${balance:.2f}, "
            f"尝试取款: ${amount:.2f}, "
            f"差额: ${shortfall:.2f}"
        )

# 测试银行账户系统
print("创建银行账户:")
try:
    # 创建账户
    account1 = BankAccount("张三", 1000)
    account2 = BankAccount("李四", 500)
    
    print(f"账户1: {account1.account_holder}, 余额: ${account1.balance:.2f}")
    print(f"账户2: {account2.account_holder}, 余额: ${account2.balance:.2f}")
    
    # 测试存款
    print("\n测试存款:")
    account1.deposit(200)
    
    # 测试取款
    print("\n测试取款:")
    account1.withdraw(100)
    
    # 测试转账
    print("\n测试转账:")
    account1.transfer(300, account2)
    
    # 测试余额不足
    print("\n测试余额不足:")
    try:
        account1.withdraw(2000)
    except InsufficientFundsError as e:
        print(f"  取款失败: {e}")
        print(f"  当前余额: ${e.balance:.2f}")
    
    # 测试无效操作
    print("\n测试无效存款金额:")
    try:
        account1.deposit(-50)
    except ValueError as e:
        print(f"  错误: {e}")
    
    # 打印对账单
    print(account1.get_statement())
    print(account2.get_statement())
    
except Exception as e:
    print(f"银行账户操作失败: {e}")

# ============================================================================
# 7. 异常处理的最佳实践总结
# ============================================================================
print("\n7. 异常处理的最佳实践总结")
print("-" * 40)

best_practices = [
    "1. 使用具体的异常类型，而不是通用的Exception",
    "2. 提供有意义的错误信息，包含相关上下文",
    "3. 使用自定义异常来表示特定的错误情况",
    "4. 使用异常链保持原始异常的上下文",
    "5. 在适当的地方处理异常，不要过度捕获",
    "6. 清理资源（使用上下文管理器）",
    "7. 使用装饰器进行通用的异常处理逻辑",
    "8. 记录异常详细信息以便调试",
    "9. 区分客户端错误和服务器错误",
    "10. 编写异常安全的代码"
]

for practice in best_practices:
    print(f"  {practice}")

# ============================================================================
# 8. 综合测试函数
# ============================================================================
print("\n8. 综合测试")
print("-" * 40)

def comprehensive_example():
    """综合测试各种raise用法"""
    results = []
    
    # 测试1: 参数验证
    try:
        def process_age(age):
            if not isinstance(age, int):
                raise TypeError(f"年龄必须是整数，收到: {type(age)}")
            if age < 0 or age > 150:
                raise ValueError(f"年龄必须在0-150之间，收到: {age}")
            return f"年龄有效: {age}"
        
        process_age("25")  # 应该触发TypeError
    except TypeError as e:
        results.append(f"测试1通过: {e}")
    
    # 测试2: 文件操作异常链
    try:
        def read_and_parse_file(filename):
            try:
                with open(filename, 'r') as f:
                    content = f.read()
                # 模拟解析
                if content == "":
                    raise ValueError("文件内容为空")
                return content
            except (FileNotFoundError, PermissionError) as e:
                raise IOError(f"无法读取文件: {filename}") from e
        
        read_and_parse_file("/nonexistent/file.txt")
    except IOError as e:
        results.append(f"测试2通过: {e}")
        if e.__cause__:
            results.append(f"  原因: {e.__cause__}")
    
    # 测试3: 业务逻辑异常
    class OrderProcessingError(Exception):
        pass
    
    try:
        def process_order(order_id, items):
            if not items:
                raise ValueError("订单不能为空")
            
            for item in items:
                if item.get("quantity", 0) <= 0:
                    raise ValueError(f"商品数量必须大于0: {item}")
            
            # 模拟处理
            return f"订单 {order_id} 处理成功"
        
        process_order(123, [])
    except ValueError as e:
        results.append(f"测试3通过: {e}")
    
    return results

# 运行综合测试
print("运行综合测试...")
test_results = comprehensive_example()
for i, result in enumerate(test_results, 1):
    print(f"  结果{i}: {result}")

print("\n" + "=" * 60)
print("Python raise语句示例结束")
print("=" * 60)