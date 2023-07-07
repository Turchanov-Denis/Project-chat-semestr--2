import React, { useState, useEffect } from "react";
import axios from "axios";
import Chat from "./component/Chat";
import JoinComponent from "./component/JoinComponent";
import './scss/style.scss'
function App() {
  const [messages, setMessages] = useState([]);
  const [characterName, setCharacterName] = useState('Rafu');
  const [login, setLogin] = useState(false);
  useEffect(() => {
    fetchMessages(); // Запускаем первоначальный запрос
    const intervalId = setInterval(fetchMessages, 10000); // Устанавливаем интервал на 5 секунд

    return () => {
      clearInterval(intervalId); // Очищаем интервал при размонтировании компонента
    };
  }, []);

  const fetchMessages = async () => {
    try {
      const response = await axios.get("http://localhost:8080/messages");
      console.log(response)
      console.log(response.data)
      setMessages(response.data);
    } catch (error) {
      console.error("Ошибка при получении сообщений:", error);
    }
  };

  const postMessage = async (message, time) => {
    try {
      await axios.post("http://localhost:8080/messages", { message, characterName, time  });
      console.log("Сообщение успешно добавлено");
      fetchMessages(); // Обновление списка сообщений после отправки нового сообщения
    } catch (error) {
      console.error("Ошибка при добавлении сообщения:", error);
    }
  };

  return (
    <div className="wrapper">
      {(login)?<Chat messages={messages} postMessage={postMessage}></Chat>:<JoinComponent onlogin={setLogin} setCharacterName={setCharacterName}></JoinComponent>}
    </div>
  );
}

export default App;
