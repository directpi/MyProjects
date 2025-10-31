package example.domain.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.UUID;

/**
 * Модель для информации о статистике игрока
 */
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class PlayerStats {
    private UUID playerId; //UUID пользователя
    private String login; //Логин пользователя
    private Long wins; //Количество побед
    private Long losses; //Количество поражений
    private Long draws; //Количество ничьих
    private Double winRatio; //Соотношение побед (wins / (losses + draws))
    
    /**
     * Конструктор для создания статистики с автоматическим расчетом соотношения
     */
    public PlayerStats(UUID playerId, String login, Long wins, Long losses, Long draws) {
        this.playerId = playerId;
        this.login = login;
        this.wins = wins;
        this.losses = losses;
        this.draws = draws;
        this.winRatio = calculateWinRatio(wins, losses, draws);
    }
    
    /**
     * Расчет соотношения побед
     */
    private Double calculateWinRatio(Long wins, Long losses, Long draws) {
        if (wins == null) wins = 0L;
        if (losses == null) losses = 0L;
        if (draws == null) draws = 0L;
        
        long totalGames = wins + losses + draws;
        if (totalGames == 0) {
            return 0.0;
        }
        
        // Соотношение побед к общему количеству игр
        return (double) wins / totalGames;
    }
}
