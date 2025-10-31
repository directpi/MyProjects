package s21.example.datasource.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import s21.example.domain.model.GameState;
import s21.example.utils.GameStateConverter;
import s21.example.utils.IntArrayConverter;

import java.time.LocalDateTime;
import java.util.UUID;

@Entity
@Table(name = "games")
@Getter
@Setter
public class GameEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private UUID id;

    @Column(name = "player1_id")
    private UUID player1Id;

    @Column(name = "player2_id", nullable = true)
    private UUID player2Id;

//    @Column(name = "state", nullable = false)
//    private String state;
    @Column(name = "state", nullable = false)
    @Convert(converter = GameStateConverter.class)
    private GameState state;

    @Convert(converter = IntArrayConverter.class)
    @Column(name = "board", nullable = false)
    private int[][] board;

    @Column(name = "created_at")
    private LocalDateTime createdAt;

    @Column(name = "updated_at")
    private LocalDateTime updatedAt;

    @Column(name = "vs_computer", nullable = false)
    private boolean vsComputer;

    @Version // Поле для оптимистичной блокировки
    //@Column(name = "version")
    @Column(columnDefinition = "integer DEFAULT 0", nullable = false)
    private Integer version;

    @PrePersist
    protected void onCreate() {
        this.createdAt = LocalDateTime.now();
        this.updatedAt = LocalDateTime.now();
    }

    @PreUpdate
    protected void onUpdate() {
        this.updatedAt = LocalDateTime.now();
    }

    public GameEntity() {}

    @Convert(converter = IntArrayConverter.class)
    @Column(name = "previous_board")
    private int[][] previousBoard = new int[3][3];
}