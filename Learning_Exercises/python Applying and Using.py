def main():
    print("-----取款-----")
    print(f"{name},你好，欢迎来到黑马银行ATM。请选择操作：")
    print ("")
name = input("请输入您的姓名：")
print("1.取款")
print("2.存款")
print("3.查询余额")
choice = input("请输入操作编号：")
if choice == "1":
    amount = float(input("请输入取款金额："))
    print(f"您已成功取款{amount}元。") 
elif choice == "2":
    amount = float(input("请输入存款金额："))
    print(f"您已成功存款{amount}元。")
elif choice == "3":
    print("您的账户余额为1000元。")
else:    print("无效的操作编号，请重新输入。")
