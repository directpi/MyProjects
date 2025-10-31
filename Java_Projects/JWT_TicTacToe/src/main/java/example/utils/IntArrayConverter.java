package example.utils;

import com.google.gson.Gson;
import jakarta.persistence.AttributeConverter;
import jakarta.persistence.Converter;

@Converter(autoApply = true)
public class IntArrayConverter implements AttributeConverter<int[][], String> {

    private static final Gson gson = new Gson();

    @Override
    public String convertToDatabaseColumn(int[][] board) {
        if (board == null) {
            return "[[0,0,0],[0,0,0],[0,0,0]]"; // Возвращаем пустую доску
        }
        return gson.toJson(board); // Преобразуем массив в JSON-строку
    }

    @Override
    public int[][] convertToEntityAttribute(String json) {
        if (json == null || json.isEmpty()) {
            return new int[3][3]; // Возвращаем пустую доску
        }
        return gson.fromJson(json, int[][].class); // Преобразуем JSON-строку в массив
    }
}