import os
import requests
from duckduckgo_search import DDGS
from urllib.parse import urlparse
import time

def search_and_download(keywords, file_type="pdf", limit=10, download_folder="无机化学复习资料"):
    # 1. 创建保存文件夹
    if not os.path.exists(download_folder):
        os.makedirs(download_folder)
        print(f"文件夹 '{download_folder}' 已创建。")

    print(f"正在搜索: {keywords} filetype:{file_type} ...")
    
    # 2. 使用 DuckDuckGo 搜索 (不需要 API Key)
    # 这里的关键词会自动加上 filetype:pdf
    query = f"{keywords} filetype:{file_type}"
    
    try:
        results = DDGS().text(query, max_results=limit)
    except Exception as e:
        print(f"搜索出错: {e}")
        return

    print(f"找到 {len(results)} 个潜在资源，准备下载...\n")

    # 3. 遍历结果并下载
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
    }

    count = 0
    for res in results:
        url = res['href']
        title = res['title']
        
        # 获取文件名
        parsed_url = urlparse(url)
        filename = os.path.basename(parsed_url.path)
        if not filename.endswith(f".{file_type}"):
            filename = f"resource_{count}.{file_type}"
        
        # 清理文件名中的非法字符
        filename = "".join([c for c in filename if c.isalpha() or c.isdigit() or c in "._- "])
        save_path = os.path.join(download_folder, filename)

        print(f"[{count+1}] 正在下载: {title} -> {filename}")
        
        try:
            # 设置超时，防止卡死
            response = requests.get(url, headers=headers, timeout=15, stream=True)
            if response.status_code == 200:
                with open(save_path, 'wb') as f:
                    for chunk in response.iter_content(chunk_size=8192):
                        f.write(chunk)
                print("    -> 下载成功")
                count += 1
            else:
                print(f"    -> 下载失败 (状态码: {response.status_code})")
        except Exception as e:
            print(f"    -> 下载出错: {e}")
        
        # 礼貌性延时，防止被封
        time.sleep(2)

    print(f"\n任务完成！共成功下载 {count} 个文件到 '{download_folder}' 文件夹。")

if __name__ == "__main__":
    # 你可以在这里修改关键词，建议多跑几次不同的关键词
    # 例如： "无机化学 期末试卷", "Inorganic Chemistry Lecture Notes", "宋天佑 无机化学 PPT"
    search_and_download("大学无机化学 复习 重点", limit=10)