import React, { useState, useEffect } from "react";
import axios from "axios";

function App() {
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    fetchMessages();
  }, []);

  const fetchMessages = async () => {
    try {
      const response = await axios.get("http://localhost:8080/messages");
      setMessages(response.data);
    } catch (error) {
      console.error("Ошибка при получении сообщений:", error);
    }
  };

  const postMessage = async (message) => {
    try {
      await axios.post("http://localhost:8080/messages", { message });
      console.log("Сообщение успешно добавлено");
      fetchMessages(); // Обновление списка сообщений после отправки нового сообщения
    } catch (error) {
      console.error("Ошибка при добавлении сообщения:", error);
    }
  };

  return (
    <div>
      <h1>Чат</h1>
      <div>
        {messages && messages.map((message, index) => (
          <p key={index}>{message}</p>
        ))}
      </div>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          const message = e.target.elements.message.value;
          postMessage(message);
          e.target.reset();
        }}
      >
        <input type="text" name="message" placeholder="Введите сообщение" />
        <button type="submit">Отправить</button>
      </form>
    </div>
  );
}

export default App;
