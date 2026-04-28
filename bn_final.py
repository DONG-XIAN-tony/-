import pandas as pd

priors = pd.read_csv('/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260413/輸入貝氏資料表（可重複使用塑膠）/output_bn_priors.csv')
cpt = pd.read_csv('/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260413/輸入貝氏資料表（可重複使用塑膠）/output_bn_cpt.csv')

all_rows = []
    
for net_id in sorted(priors['network_id'].unique()):                            #對每個網路ID進行處理
    net_name = priors[priors['network_id'] == net_id]['network_name'].values[0] #取得網路名稱
    root_row = priors[priors['network_id'] == net_id].iloc[0]                   #取得根節點資料（每個網路只有一個根節點，且在priors中是第一行）
    root_name = root_row['node_name']
    p_root = root_row['P(會發生)']
    
    net_cpt = cpt[cpt['network_id'] == net_id].sort_values('parent_order')
    chain = [(root_name, p_root)]
    current_name = root_name
    current_p = p_root
    visited = set([current_name])
    
    # 沿鏈傳遞機率：全機率公式
    while True:
        row = net_cpt[net_cpt['parent_name'] == current_name]
        if row.empty:
            break
        row = row.iloc[0]
        child_name = row['child_name']
        if child_name in visited:
            break
        
        # P(child=會) = P(child=會|parent=會) * P(parent=會)
        #             + P(child=會|parent=不會) * P(parent=不會)
        p_child = (row['P(child=會發生|parent=會發生)'] * current_p 
                 + row['P(child=會發生|parent=不會發生)'] * (1 - current_p))
        
        chain.append((child_name, p_child))
        visited.add(child_name)
        current_name = child_name       #更新當前節點為子節點 繼續往下傳遞
        current_p = p_child             #更新當前機率為子節點的機率，繼續往下傳遞
    
    for i, (name, p) in enumerate(chain):
        all_rows.append({
            'network_id': net_id,
            'network_name': net_name,
            'node_order': i + 1,
            'node_name': name,
            'P(會發生)': round(p, 6),
            'is_root': (i == 0),
            'is_final_child': (i == len(chain) - 1) and (i != 0)
        })
    
df_out = pd.DataFrame(all_rows)
df_out.to_csv('/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260413/最終節點貝氏機率產出/輸出結果/bn_risk_chain_probabilities.csv', index=False, encoding='utf-8-sig')

# 只輸出最終子節點摘要
summary = df_out[df_out['is_final_child'] == True][
    ['network_id', 'network_name', 'node_name', 'P(會發生)']
].copy()
summary.columns = ['network_id', 'network_name', '最終子節點', 'P(最終子節點=會發生)']
summary.to_csv('/Users/tonykk90918/Documents/台灣大學研究所/環境規劃管理實驗室/碩論/程式碼/20260413/最終節點貝氏機率產出/輸出結果/bn_final_node_summary.csv', index=False, encoding='utf-8-sig')

print(summary.to_string(index=False))