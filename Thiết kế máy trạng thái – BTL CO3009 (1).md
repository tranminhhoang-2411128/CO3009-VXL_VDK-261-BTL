# Thiết kế máy trạng thái – BTL CO3009

Hệ thống đèn giao thông 2 tuyến trên XNUCLEO-F103RB. Thời gian mặc định: Đỏ 10s, Xanh 7s, Vàng 3s (Đỏ = Xanh + Vàng). Cách gán nút bên dưới là đề xuất, cần đối chiếu với Project\_Group.docx.

## 1. Sơ đồ chuyển chế độ

Ba chế độ lớn chuyển vòng tròn bằng BTN0.

| Từ | Sang | Nút | Ghi chú |
| --- | --- | --- | --- |
| Reset | Tự động | – | Vào AUTO\_INIT |
| Tự động | Bằng tay | BTN0 | Hủy timer tự động |
| Bằng tay | Chỉnh thời gian | BTN0 | Cả 2 tuyến Đỏ |
| Chỉnh thời gian | Tự động | BTN0 | Lưu nếu hợp lệ, rồi về AUTO\_INIT |

## 2. Chế độ tự động

Bốn trạng thái chạy vòng tròn. Mỗi lần chuyển là một lần `timer_flag[0]` bật lên.

| Trạng thái | Tuyến 1 | Tuyến 2 | Thời gian | Trạng thái kế tiếp |
| --- | --- | --- | --- | --- |
| AUTO\_RED1\_GREEN2 | Đỏ | Xanh | 7s | AUTO\_RED1\_YELLOW2 |
| AUTO\_RED1\_YELLOW2 | Đỏ | Vàng | 3s | AUTO\_GREEN1\_RED2 |
| AUTO\_GREEN1\_RED2 | Xanh | Đỏ | 7s | AUTO\_YELLOW1\_RED2 |
| AUTO\_YELLOW1\_RED2 | Vàng | Đỏ | 3s | AUTO\_RED1\_GREEN2 |

Chu kỳ đầy đủ là 7 + 3 + 7 + 3 = 20s. Đỏ của mỗi tuyến kéo dài 10s, đúng bằng Xanh + Vàng của tuyến kia. AUTO\_INIT chỉ chạy một lần: đặt T1 Đỏ, T2 Xanh, setTimer(0, time\_green) rồi vào AUTO\_RED1\_GREEN2.

## 3. Chế độ bằng tay&#32;

| Trạng thái | T1 | T2 | Vào bằng |
| --- | --- | --- | --- |
| MAN\_ALL\_RED | Đỏ | Đỏ | BTN0 từ tự động (an toàn, không có 2 tuyến Xanh) |
| MAN\_RED1\_GREEN2 | Đỏ | Xanh | BTN1 |
| MAN\_RED1\_YELLOW2 | Đỏ | Vàng | BTN1 |
| MAN\_GREEN1\_RED2 | Xanh | Đỏ | BTN1 |
| MAN\_YELLOW1\_RED2 | Vàng | Đỏ | BTN1 (rồi quay lại MAN\_RED1\_GREEN2) |

Mỗi lần bấm BTN1 chỉ đi theo đúng thứ tự trên nên hai tuyến không bao giờ cùng Xanh. Khi sang chế độ này cần hủy timer của chế độ tự động.

## 4. Chỉnh thời gian

| Nút | Tác dụng |
| --- | --- |
| BTN1 | Chọn đèn cần chỉnh: Đỏ, Xanh, Vàng |
| BTN2 | Tăng 1s (trên biến tạm) |
| BTN3 | Giảm 1s (trên biến tạm) |
| BTN0 | Xác nhận và thoát: hợp lệ thì lưu và về tự động; không hợp lệ thì báo lỗi trên LCD và ở lại |

Khi vào chế độ này, đèn nên nháy hoặc giữ đèn đang chỉnh; LCD hiển thị tên đèn và giá trị đang sửa.

Ràng buộc hợp lệ:

- Mọi giá trị trong khoảng 1s đến 99s.
- Đỏ = Xanh + Vàng.
- Chỉnh Đỏ thì Xanh = Đỏ − Vàng (phải còn ít nhất 1s).
- Chỉnh Xanh hoặc Vàng thì Đỏ = Xanh + Vàng.
- Giá trị vi phạm thì không lưu, giữ giá trị cũ và báo lỗi ngắn trên LCD.
- Chỉ ghi vào time\_red, time\_green, time\_yellow khi người dùng xác nhận.

## 5. Bốn câu hỏi khi chuyển chế độ

| Chuyển | Đèn sau khi chuyển | Timer | LCD |
| --- | --- | --- | --- |
| Tự động → Bằng tay | Cả 2 Đỏ | Hủy timer tự động | "MANUAL" |
| Bằng tay → Chỉnh | Cả 2 Đỏ | Không timer đèn | Tên đèn và giá trị |
| Chỉnh → Tự động | Về AUTO\_INIT (T1 Đỏ, T2 Xanh) | setTimer lại bằng giá trị mới | Nhiệt độ, độ ẩm |

Mọi lần vào chế độ mới đều đi qua trạng thái đã biết là an toàn, nên không xảy ra trường hợp đang Vàng mà chuyển chế độ rồi có 2 tuyến Xanh.

## 6. Tổng hợp yêu cầu

Máy trạng thái chiếm 30% điểm, cao nhất trong các tiêu chí, nên bảng dưới gom mọi yêu cầu liên quan tới FSM, đèn và LCD.

| Nhóm | Yêu cầu | Nguồn |
| --- | --- | --- |
| Scheduler | Toàn bộ hệ thống chạy trên scheduler O(1) với `SCH_Add_Task`, `SCH_Delete_Task`, `SCH_Update` (O(1)), `SCH_Dispatch_Tasks`; hỗ trợ one-shot task | Thông báo, A2 và Bước 4 |
| Chế độ tự động | Hai tuyến Đỏ – Xanh – Vàng luân phiên theo thời gian cài đặt (mặc định Đỏ 10s, Xanh 7s, Vàng 3s) | Thông báo, A2 |
| Chế độ bằng tay | Người dùng chuyển trạng thái đèn bằng nút nhấn | Thông báo, A2 |
| Chỉnh thời gian | Đổi thời gian Đỏ, Xanh, Vàng bằng nút nhấn, hiển thị trên LCD; có ràng buộc hợp lệ và cách xử lý khi nhập sai | Thông báo, A2 và 6.1 |
| Môi trường | Đọc DHT20, hiển thị nhiệt độ và độ ẩm trên LCD | Thông báo, A2 |
| Bố cục LCD | LCD 2 dòng: dòng trên hiển thị MODE, dòng dưới hiển thị nhiệt độ + độ ẩm; dòng dưới luôn hiển thị và cập nhật dù đổi mode đèn | Yêu cầu bổ sung của nhóm |
| Chuyển chế độ | Mỗi lần chuyển phải trả lời được: đèn sau khi chuyển, timer và task khởi tạo lại thế nào, LCD hiển thị gì, và không bao giờ có 2 tuyến cùng Xanh | Thông báo, 6.1 |
| Tình huống biên | Thử nhấn nút liên tục, chuyển chế độ đúng lúc đèn Vàng, nhập thời gian không hợp lệ | Thông báo, Bước 6 |

## 7. Quy tắc hiện thực

Các quy tắc dưới đây là điều kiện chấm điểm phần tổ chức mã nguồn và máy trạng thái.

- Mỗi FSM là một cặp `.h/.c` riêng (`fsm_automatic`, `fsm_manual`, `fsm_tuning`); hàm chạy FSM được gọi như một task của scheduler.
- FSM không chứa lệnh truy xuất phần cứng (`HAL_GPIO_...`, `HAL_I2C_...`). Mọi thao tác đèn, nút, LCD đều qua hàm của các module phụ trợ.
- Chỉ các module phụ trợ và `main.c` được gọi HAL.
- Biến dùng chung (trạng thái hiện tại, thời gian Đỏ/Xanh/Vàng) khai báo trong `global.h/global.c`.
- Không dùng `HAL_Delay` trong chương trình chính; mọi công việc chạy dưới dạng task.
- Thiết kế trước, lập trình sau: vẽ sơ đồ 3 FSM và FSM chuyển chế độ, thống nhất trong nhóm rồi mới viết mã.
- Kiểm thử từng lớp độc lập từ dưới lên: phần cứng, scheduler, module phụ trợ, rồi FSM.
- Quy ước đặt tên: tên file chữ thường nối bằng gạch dưới (`fsm_automatic.c`), tên hàm dạng `<module>_<hành_động>` (`button_read()`, `traffic_set_state()`).
- Commit thường xuyên bằng tài khoản của chính mình, mỗi commit một thay đổi có ý nghĩa.

## 8. Hiển thị LCD

Dòng trên của LCD hiển thị MODE, dòng dưới hiển thị nhiệt độ và độ ẩm, và dòng dưới luôn cập nhật dù đèn đang ở mode nào. Ở mục 4 và 5, mọi nội dung LCD do FSM đặt chỉ nằm ở dòng trên.

| Dòng | Nội dung ví dụ | Ai quyết định |
| --- | --- | --- |
| Trên | `MODE: AUTO`, `MODE: MANUAL`, `TUNE RED: 10s`, `INVALID` | FSM đặt chuỗi `lcd_top` |
| Dưới | `T:28.5C H:65%` | Task DHT20, độc lập với mọi mode |

Ba task độc lập (tên hàm là đề xuất):

| Task | Chu kỳ | Nhiệm vụ |
| --- | --- | --- |
| `dht20_start` | 2s (200 tick) | Gửi lệnh đo |
| `dht20_finish` | One-shot, 100 ms (10 tick) sau `dht20_start` | Đọc kết quả, cập nhật biến toàn cục |
| `lcd_display_run` | 200 ms (20 tick) | Nơi duy nhất ghi LCD, chỉ vẽ lại dòng nào có thay đổi |

Quy tắc cho phần hiển thị:

- FSM chỉ đặt chuỗi `lcd_top`, không gọi `lcd_clear()`.
- Mỗi dòng được đệm đủ 16 ký tự thay cho việc xóa màn hình, để không sót ký tự cũ và không nhấp nháy.
- Cảm biến lỗi hoặc chưa có số liệu thì dòng dưới hiện `T:--.-C H:--%`, không giữ số cũ.
- DHT20 cần chờ khoảng 80 ms sau lệnh đo (kiểm tra lại datasheet) mà không được dùng `HAL_Delay`, nên tách thành start rồi one-shot finish.
- DHT20 và LCD dùng chung I2C1. Scheduler chạy tuần tự và không có lệnh I2C trong ngắt nên hai task không đụng nhau; chỉ ghi LCD khi chuỗi đổi để task đọc nút không bị trễ nhịp.
- Báo lỗi nhập sai: đặt `lcd_top` thành `INVALID`, rồi một one-shot task (khoảng 1,5s) trả về dòng chỉnh thời gian.

```c
extern char    lcd_top[17];   // dòng trên, FSM ghi
extern int16_t g_temp_x10;    // 285 nghĩa là 28.5 độ C
extern uint8_t g_humi;        // %
extern uint8_t g_sensor_ok;   // 0 nếu đọc lỗi hoặc chưa có số liệu

void lcd_display_run(void) {
    char top[17], bot[17], tmp[17];
    snprintf(top, 17, "%-16s", lcd_top);
    if (g_sensor_ok)
        snprintf(tmp, 17, "T:%d.%dC H:%u%%", g_temp_x10 / 10, g_temp_x10 % 10, g_humi);
    else
        snprintf(tmp, 17, "T:--.-C H:--%%");
    snprintf(bot, 17, "%-16s", tmp);
    if (strcmp(top, last_top)) { lcd_put_line(0, top); strcpy(last_top, top); }
    if (strcmp(bot, last_bot)) { lcd_put_line(1, bot); strcpy(last_bot, bot); }
}
```

## 9. Module sử dụng

FSM chỉ gọi hàm của các module phụ trợ, còn các module phụ trợ mới là nơi gọi HAL. Tên hàm trong cột 3 bám theo gợi ý của đề hoặc là đề xuất, bạn đổi cho khớp với code của nhóm.

| Module | File | Hàm chính | Ai dùng |
| --- | --- | --- | --- |
| Scheduler | `scheduler.h/.c` | `SCH_Init`, `SCH_Add_Task`, `SCH_Delete_Task`, `SCH_Update`, `SCH_Dispatch_Tasks` | `main.c`, mọi task |
| Software timer | `software_timer.h/.c` | `setTimer`, `timer_flag[]`; cập nhật bởi task chu kỳ 10 ms | 3 FSM |
| Button | `button.h/.c` | `isButtonPressed(i)`; đọc nút mỗi 10 ms, chống dội, phát hiện nhấn và nhấn giữ | 3 FSM |
| Đèn giao thông | `traffic_light.h/.c` | `traffic_set(tuyến, màu)`, dùng bảng tổ hợp tín hiệu ở mục 3.2 của thông báo | 3 FSM |
| DHT20 | `dht20.h/.c` | Giao tiếp I2C, đọc và đổi nhiệt độ, độ ẩm; đọc theo chu kỳ, không đọc liên tục | Task `dht20_start`, `dht20_finish` |
| LCD | `lcd.h/.c` | Khởi tạo, xóa, đặt con trỏ, in chuỗi; `lcd_put_line(dòng, chuỗi)`; dùng địa chỉ tìm được ở bước quét I2C | `lcd_display_run` |
| Hiển thị LCD | `lcd_display.h/.c` (đề xuất) | `lcd_display_run` | Scheduler |
| Biến dùng chung | `global.h/.c` | Trạng thái hiện tại, `time_red`, `time_green`, `time_yellow`, `lcd_top`, `g_temp_x10`, `g_humi`, `g_sensor_ok` | Tất cả |
| FSM | `fsm_automatic`, `fsm_manual`, `fsm_tuning` (`.h/.c`) | `fsm_*_run`, gọi như task | Scheduler |

## 10. Ranh giới giữa FSM và các module

FSM chỉ được dùng bốn module ở bảng 10.1; mọi module hoặc lệnh ngoài bảng đều bị cấm trong `fsm_*.c`. Ngược lại, code của các module ở bảng 10.1 không được chứa logic của FSM. Khi hiện thực, viết các module ở bảng 10.1 trước, rồi FSM chỉ gọi chúng: không viết lại phần việc của module bên trong FSM, và không đưa logic chuyển trạng thái vào module.

### 10.1 Module FSM được phép dùng

| Module | Hardware phía sau | FSM được gọi hoặc đọc | Module chịu trách nhiệm | Không được đặt trong module này |
| --- | --- | --- | --- | --- |
| `software_timer` | Không có (đếm theo tick 10 ms) | `setTimer(i, tick)`, `clearTimer(i)`, `timer_flag[i]` | Đếm thời gian, bật cờ khi hết hạn | Việc chọn trạng thái kế tiếp hoặc thời gian của đèn |
| `button` | Module nút nhấn 1 và 2 (D7 – D10) | `isButtonPressed(i)` | Đọc nút mỗi 10 ms, chống dội, phát hiện nhấn và nhấn giữ | Việc mỗi nút làm gì theo từng mode |
| `traffic_light` | Module đèn giao thông 1 và 2 (D3 – D6) | `traffic_set(tuyến, màu)` | Đổi (tuyến, màu) thành tổ hợp 2 tín hiệu GPIO theo bảng ở mục 3.2 của thông báo | Timer, thứ tự các trạng thái đèn |
| `global` | Không có | `status`, `time_red`, `time_green`, `time_yellow`, các enum trạng thái, `lcd_top` (FSM ghi chuỗi dòng trên của LCD) | Giữ biến dùng chung | Hàm xử lý logic |

Tên hàm bám theo gợi ý của đề; `clearTimer` là đề xuất, thêm vào `software_timer` nếu chưa có.

### 10.2 Module và lệnh FSM không được dùng

| Cấm trong `fsm_*.c` | Lý do | Ai làm việc đó |
| --- | --- | --- |
| `HAL_*` (`HAL_GPIO_...`, `HAL_I2C_...`, `HAL_Delay`) | Quy tắc hiện thực ở mục 7 | Các module phụ trợ và `main.c` |
| `lcd.h` (`lcd_put_line`, `lcd_clear`, ...) | FSM chỉ đặt chuỗi `lcd_top`, không ghi LCD trực tiếp | `lcd_display_run` |
| `dht20` và `g_temp_x10`, `g_humi`, `g_sensor_ok` | Dòng dưới của LCD độc lập với mode đèn | `dht20_start`, `dht20_finish`, `lcd_display_run` |
| `scheduler`: `SCH_Init`, `SCH_Update`, `SCH_Dispatch_Tasks`, `SCH_Delete_Task` | FSM chỉ là một task do scheduler gọi | `main.c` và scheduler |

Ngoại lệ duy nhất: `fsm_tuning` được gọi `SCH_Add_Task` để đặt one-shot trả dòng trên về lại màn hình chỉnh thời gian sau khi báo `INVALID` (mục 8).
