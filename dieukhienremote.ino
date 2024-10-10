#include <DHT.h>               // Thư viện cho cảm biến nhiệt độ
#include <IRremote.h>          // Thư viện cho remote hồng ngoại
// Định nghĩa các chân kết nối
#define DHTPIN      2          // Chân cho cảm biến DHT
#define DHTTYPE     DHT11      // Loại cảm biến (DHT11 hoặc DHT22)
#define RELAY_PIN   7          // Pin kết nối với relay hoặc quạt
#define IR_RECEIVE_PIN 3       // Pin kết nối với bộ thu hồng ngoại
DHT dht(DHTPIN, DHTTYPE);      // Khởi tạo cảm biến DHT
IRrecv irrecv(IR_RECEIVE_PIN); // Khởi tạo bộ thu hồng ngoại
decode_results results;        // Lưu trữ kết quả của remote
float thresholdTemp = 30.0;    // Ngưỡng nhiệt độ để bật relay
bool relayState = false;       // Trạng thái của relay
void setup() {
  Serial.begin(9600);
  dht.begin();                 // Bắt đầu cảm biến DHT
  irrecv.enableIRIn();         // Bắt đầu bộ thu hồng ngoại
  pinMode(RELAY_PIN, OUTPUT);  // Cài đặt relay là ngõ ra
  digitalWrite(RELAY_PIN, LOW); // Tắt relay ban đầu
}
void loop() {
  // Đọc giá trị nhiệt độ từ DHT
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Lỗi đọc cảm biến!");
  } else {
    Serial.print("Nhiệt độ: ");
    Serial.println(temp);
    // Kiểm tra nhiệt độ và bật/tắt relay
    if (temp >= thresholdTemp) {
      digitalWrite(RELAY_PIN, HIGH);  // Bật relay khi nhiệt độ lớn hơn ngưỡng
      relayState = true;
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Tắt relay khi nhiệt độ nhỏ hơn ngưỡng
      relayState = false;
    }
  }
  if (irrecv.decode(&results)) {
    Serial.print("Mã remote: ");
    Serial.println(results.value, HEX);
    // Ví dụ: Bật relay khi nhận mã từ remote
    if (results.value == 0xFF30CF) {  // Giả sử đây là mã nút bật relay
      relayState = true;
      digitalWrite(RELAY_PIN, HIGH);  // Bật relay
    }
    // Tắt relay khi nhận mã khác từ remote
    if (results.value == 0xFF18E7) {  // Giả sử đây là mã nút tắt relay
      relayState = false;
      digitalWrite(RELAY_PIN, LOW);   // Tắt relay
    }
    irrecv.resume();  // Chuẩn bị nhận tín hiệu kế tiếp
  }
  delay(1000);  // Chờ 1 giây trước khi lặp lại
}
