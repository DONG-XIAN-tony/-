from openai import OpenAI
from pathlib import Path
import time
import os


#直接從環境變數 的OPENAI_API_KEY讀取 API金鑰 我存在shell
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

# 接下來我要把我電腦內的pdf檔案上傳給chatgpt做學術文本分析 
# 列出資料夾內所有 PDF 檔案
def list_pdfs(folder: str):
    p = Path(folder) # 建立 Path 物件 之後能找出符合的檔案
    if not p.exists():
        raise FileNotFoundError(f"資料夾不存在: {folder}")
    pdfs = sorted(p.glob("*.pdf"))  # 找出該資料夾下所有.pdf 並排序 （讓之後重複試驗 固定檔案 輸出）
    return pdfs

# 上傳單一 PDF，回傳在openai 內的 file_id 
def upload_pdf(pdf_path: Path) -> str:
    """
    上傳單一 PDF 回傳 file_id
    """
    with open(pdf_path, "rb") as f:
        file_obj = client.files.create(  #在客戶端檔案夾中創造一個上傳pdf的檔案夾
            file=f,
            purpose="user_data",  #用途標籤 給模型讀檔/分析用途
        )
    return file_obj.id #回傳上傳後的 file_id


# 把 PDF (file_id) 丟給模型，回傳文字結果
def analyze_pdf(file_id: str, prompt: str, model: str = "gpt-4o") -> str:
    """
    把 PDF (file_id) 丟給模型，回傳文字結果
    """
    resp = client.responses.create(
    model=model,
    temperature=0.8,
    input=[
        {
            "role": "user",
            "content": [
                {"type": "input_file", "file_id": file_id},
                {"type": "input_text", "text": prompt},
            ],
        }
    ],
)
    return resp.output_text # 回傳模型的文字回應




def analyze_pdf_multiple_runs(
    file_id: str,
    pdf_stem: str,
    prompt: str,
    out_dir: Path,
    model: str,
    runs: int = 5,
    sleep_sec: int = 65,  # ⚠️ 對齊 30k TPM
):
    run_texts = []

    for r in range(1, runs + 1):
        print(f"      ▶ Run {r}/{runs} 分析中...")

        text = analyze_pdf(
            file_id=file_id,
            prompt=prompt,
            model=model,
        )

        run_file = out_dir / f"{pdf_stem}_run_{r:02d}.txt"
        run_file.write_text(text, encoding="utf-8")

        run_texts.append(
            f"\n{'='*40}\nRUN {r}\n{'='*40}\n{text}\n"
        )

        print(f" ⏳ Sleep {sleep_sec}s（TPM 保護）")
        time.sleep(sleep_sec)

    # 彙整該 PDF 的 5 次結果
    aggregate_text = "\n".join(run_texts)
    aggregate_file = out_dir / f"{pdf_stem}_AGGREGATE.txt"
    aggregate_file.write_text(aggregate_text, encoding="utf-8")

    return aggregate_text


# 批次上傳並分析資料夾內所有 PDF
def batch_upload_and_analyze(
    pdf_folder: str,
    out_folder: str,
    prompt: str,
    model: str = "gpt-4o", #預設模型
):
    outp = Path(out_folder)
    outp.mkdir(parents=True, exist_ok=True)

    pdfs = list_pdfs(pdf_folder)
    if not pdfs:
        print("找不到任何 PDF")
        return

    all_pdf_aggregates = []

    for i, pdf_path in enumerate(pdfs, start=1):
        print(f"[{i}/{len(pdfs)}] 上傳: {pdf_path.name}")

        pdf_out_dir = outp / pdf_path.stem
        pdf_out_dir.mkdir(exist_ok=True)

        try:
            file_id = upload_pdf(pdf_path)
            print(f"    file_id={file_id}")

            aggregate_text = analyze_pdf_multiple_runs(
                file_id=file_id,
                pdf_stem=pdf_path.stem,
                prompt=prompt,
                out_dir=pdf_out_dir,
                model=model,
                runs=5,
                sleep_sec=65,  # ⚠️ 30k TPM
            )

            all_pdf_aggregates.append(
                f"\n{'#'*60}\nPDF: {pdf_path.name}\n{'#'*60}\n{aggregate_text}\n"
            )

        except Exception as e:
            print(f"    ❌ 失敗: {pdf_path.name} -> {e}")

    # === 全部 PDF 總彙整 ===
    final_file = outp / "ALL_PDFS_AGGREGATE.txt"
    final_file.write_text("\n".join(all_pdf_aggregates), encoding="utf-8")


# 讀取風險分析 prompt 因為還沒定案 所以先用路徑 + Path 讀取 之後用import 
ANALYSIS_PROMPT = Path("/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/prompt/prompt20260112.txt").read_text(encoding="utf-8")


if __name__ == "__main__":
    PDF_FOLDER = "/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260112/輸入文檔pdf直接讀取"
    OUT_FOLDER = "/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260112/輸出文檔pdf直接讀取/2"

    ANALYSIS_PROMPT = ANALYSIS_PROMPT.strip()

    batch_upload_and_analyze(
        pdf_folder=PDF_FOLDER,
        out_folder=OUT_FOLDER,
        prompt=ANALYSIS_PROMPT,
        model="gpt-4o",
    )