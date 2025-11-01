#include <map>
using namespace std;

class Stock {
private:
    multimap<int, pair<int, int>> weight_map; // Грузоподъемность -> {объем, -id}
    multimap<int, pair<int, int>> volume_map; // Объем -> {грузоподъемность, -id}
    int next_id = 0;                          // Следующий серийный номер коробки

public:
    void Add(int w, int v) {
        weight_map.emplace(w, make_pair(v, -next_id));
        volume_map.emplace(v, make_pair(w, -next_id));
        next_id++;
    }

    int GetByW(int min_w) {
        // Найти первую подходящую коробку по грузоподъемности
        auto it = weight_map.lower_bound(min_w);
        if (it == weight_map.end())
            return -1;

        // Извлекаем информацию о коробке
        int best_id = -it->second.second;  // Извлекаем id коробки
        int volume = it->second.first;    // Объем коробки

        // Удаляем коробку из weight_map
        weight_map.erase(it);

        // Удаляем коробку из volume_map
        auto range = volume_map.equal_range(volume);
        for (auto volume_it = range.first; volume_it != range.second; ++volume_it) {
            if (volume_it->second.second == -best_id) {
                volume_map.erase(volume_it);
                break;
            }
        }

        return best_id;
    }

    int GetByV(int min_v) {
        // Найти первую подходящую коробку по объему
        auto it = volume_map.lower_bound(min_v);
        if (it == volume_map.end())
            return -1;

        // Извлекаем информацию о коробке
        int best_id = -it->second.second;  // Извлекаем id коробки
        int weight = it->second.first;    // Грузоподъемность коробки

        // Удаляем коробку из volume_map
        volume_map.erase(it);

        // Удаляем коробку из weight_map
        auto range = weight_map.equal_range(weight);
        for (auto weight_it = range.first; weight_it != range.second; ++weight_it) {
            if (weight_it->second.second == -best_id) {
                weight_map.erase(weight_it);
                break;
            }
        }

        return best_id;
    }
};