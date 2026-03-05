import requests
from bs4 import BeautifulSoup

def main():
    # 1. 目标网址
    url = "https://movie.douban.com/top250"
    
    # 2. 伪装头（必填，否则会被拦截）
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
    }

    print("正在向豆瓣发送请求...")
    
    try:
        # 3. 发送请求
        response = requests.get(url, headers=headers)
        
        # 如果状态码不是200，说明出错了
        if response.status_code != 200:
            print(f"请求失败，状态码：{response.status_code}")
            return

        # 4. 解析网页
        soup = BeautifulSoup(response.text, "html.parser")
        
        # 5. 提取列表（找到包含25部电影的每一个“大框框”）
        # 对应 F12 里的 <div class="item">
        movie_list = soup.find_all("div", class_="item")

        print(f"成功获取到 {len(movie_list)} 部电影，结果如下：")
        print("=" * 50)

        # 6. 循环遍历每一部电影
        for movie in movie_list:
            # --- 提取标题 ---
            # 对应 <span class="title">肖申克的救赎</span>
            # find 找第一个匹配的标签
            title_tag = movie.find("span", class_="title")
            title = title_tag.text if title_tag else "未知标题"

            # --- 提取评分 ---
            # 对应 <span class="rating_num">9.7</span>
            score_tag = movie.find("span", class_="rating_num")
            score = score_tag.text if score_tag else "无评分"

            # --- 提取链接 ---
            # 对应 <a href="...">
            link_tag = movie.find("a")
            link = link_tag["href"] if link_tag else "无链接"

            # --- 提取一句话评价 (可选) ---
            # 对应 <span class="inq">希望让人自由</span>
            quote_tag = movie.find("span", class_="inq")
            quote = quote_tag.text if quote_tag else ""

            # 打印结果
            print(f"电影：{title}")
            print(f"评分：{score}")
            if quote:
                print(f"评价：{quote}")
            print(f"链接：{link}")
            print("-" * 30)

    except Exception as e:
        print(f"程序运行出错：{e}")

if __name__ == "__main__":
    main()