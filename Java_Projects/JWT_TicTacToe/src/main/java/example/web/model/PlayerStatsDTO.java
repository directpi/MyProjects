package example.web.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.UUID;

/**
 * DTO для передачи информации о статистике игрока в API
 */
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class PlayerStatsDTO {
    private UUID playerId; // UUID пользователя
    private String login; // Логин пользователя
    private Long wins; // Количество побед
    private Long losses; // Количество поражений
    private Long draws; // Количество ничьих
    private Long totalGames; // Общее количество игр
    private Double winRatio; // Соотношение побед (от 0.0 до 1.0)
    private Double winPercentage; // Соотношение побед в процентах

    /**
     * Конструктор с автоматическим расчетом дополнительных полей
     */
    public PlayerStatsDTO(UUID playerId, String login, Long wins, Long losses, Long draws, Double winRatio) {
        this.playerId = playerId;
        this.login = login;
        this.wins = wins != null ? wins : 0L;
        this.losses = losses != null ? losses : 0L;
        this.draws = draws != null ? draws : 0L;
        this.winRatio = winRatio != null ? winRatio : 0.0;

        this.totalGames = this.wins + this.losses + this.draws;
        this.winPercentage = this.winRatio * 100.0;
    }
}
