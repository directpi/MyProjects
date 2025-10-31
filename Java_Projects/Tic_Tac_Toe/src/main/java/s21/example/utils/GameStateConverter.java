package s21.example.utils;

import jakarta.persistence.AttributeConverter;
import jakarta.persistence.Converter;
import s21.example.domain.model.GameState;

@Converter(autoApply = true)
public class GameStateConverter implements AttributeConverter<GameState, String> {

    @Override
    public String convertToDatabaseColumn(GameState state) {
        return state != null ? state.toString() : null;
    }

    @Override
    public GameState convertToEntityAttribute(String dbData) {
        return dbData != null ? GameState.valueOf(dbData) : null;
    }
}