import React, { useState, useEffect } from "react";
import axios from "axios";
import Chat from "./component/Chat";
import './scss/style.scss'
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
    <div className="wrapper">
      <Chat messages={messages} postMessage={postMessage}></Chat>
    </div>
  );
}

export default App;
