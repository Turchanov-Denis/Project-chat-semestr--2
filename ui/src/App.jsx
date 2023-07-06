import React, { useState, useEffect } from "react";
import axios from "axios";

const ChatClient = () => {
  const [messages, setMessages] = useState([]);
  const [newMessage, setNewMessage] = useState("");

  // Получение списка сообщений чата при загрузке компонента
  useEffect(() => {
    fetchMessages();
  }, []);

  // Функция для получения списка сообщений чата с сервера
  const fetchMessages = async () => {
    try {
      const response = await axios.get("http://localhost:8080/messages");
      setMessages(response.data.split("\n"));
    } catch (error) {
      console.error("Ошибка при получении сообщений:", error);
    }
  };

  // Функция для отправки нового сообщения в чат
  const sendMessage = async () => {
    try {
      await axios.post("http://localhost:8080/messages", newMessage);
      setNewMessage(""); // Сброс поля ввода
      fetchMessages(); // Обновление списка сообщений
    } catch (error) {
      console.error("Ошибка при отправке сообщения:", error);
    }
  };

  return (
    <div>
      <h1>Чат</h1>
      <div>
        {messages.map((message, index) => (
          <div key={index}>{message}</div>
        ))}
      </div>
      <input
        type="text"
        value={newMessage}
        onChange={(e) => setNewMessage(e.target.value)}
      />
      <button onClick={sendMessage}>Отправить</button>
    </div>
  );
};

export default ChatClient;
