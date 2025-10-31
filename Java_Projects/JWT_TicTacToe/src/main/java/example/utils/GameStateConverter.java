package example.utils;

import example.domain.model.GameState;
import jakarta.persistence.AttributeConverter;
import jakarta.persistence.Converter;

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