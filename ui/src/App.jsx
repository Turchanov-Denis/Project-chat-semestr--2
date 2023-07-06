import { useEffect, useState } from 'react'
import './scss/style.scss'
import JoinComponent from './component/JoinComponent';
import { useSelector, useDispatch } from 'react-redux'
import { changeAuth, setUsers,setMessages,updateMessages } from './redux/mainSlicer';
// import { io } from 'socket.io-client'
import axios from 'axios';
import socket from './socket'
import Chat from './component/Chat';
function App() {
  const joined = useSelector((state) => state.main.joined)
  const users = useSelector((state) => state.main.users)
  const messages = useSelector((state) => state.main.messages)
  const userName = useSelector((state) => state.main.userName)
  const dispatch = useDispatch()

  const onlogin = async ({ roomId, userName }) => {
    await axios.post('http://localhost:3000/rooms', { roomId, userName })
    socket.emit('ROOM:JOIN', { roomId, userName })
    const res = await axios.get(`http://localhost:3000/rooms/${roomId}`);
    console.log("resR",res.data);
    if (res.data.messages.length > 0 ) {dispatch(setMessages(res.data.messages))}
    
    dispatch(setUsers({ users: [...res.data.users] }))
    dispatch(changeAuth({ roomId, userName }))
    
  }
  const messageHelper = (messages) => {
    console.log("Rafu", messages)
    dispatch(updateMessages(messages))
  }
  useEffect(() => {
    // onlogin({ roomId: 1, userName: 'Rafu' }) // point of entry, later throw in login page
    // это прослушивание работает у всех кроме отправителя
    socket.on('ROOM:SET_USERS', (users) => {
      dispatch(setUsers({ users }))
      // console.log("RAfu")
      console.log("useEfCheck",users)
    })
    socket.on('ROOM:NEW_MESSAGE', (messages) => {
      console.log('ROOM:NEW_MESSAGE-Check',messages);
      dispatch(updateMessages(messages))
    })
  }, [])

  return (
    <>
      <div className='wrapper'>
        {(!joined) ? <JoinComponent onlogin={onlogin}></JoinComponent> : <Chat messageHelper={messageHelper}   userName={userName} messages={messages} users={users}></Chat>}
        {/* {!joined && <JoinComponent onlogin={onlogin}></JoinComponent>} */}

      </div>
    </>
  )
}

export default App
