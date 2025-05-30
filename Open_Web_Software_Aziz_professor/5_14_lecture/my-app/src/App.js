import logo from './logo.svg';
import './App.css';

function App() {
  const student = [
    { name: "Emma", major: "Computer Science", gpa:3.2, subjects: ["Algorithms","Databases","AI"] }, 
    { name: "John", major: "Mathematics", gpa:3.8, subjects: ["Calculus","Linear Algebra","Statistics"] },
    { name: "Sophia", major: "Physics", gpa:3.5, subjects: ["Quantum Mechanics","Thermodynamics","Electromagnetism"] },
  ]
  const shoot = () => {
    alert("Great Shot");
  
  }

  const studentList = student.map((std) => {
    return (
      <div>
        <h3>{std.name}</h3>
        <p>Major: {std.major}</p>
        <p>GPA: {std.gpa}</p>
        <p>Subjects: {std.subjects}</p>
      </div>
    )
  })
  return (
    <div>
      <h3>Student Profile</h3>
      <button onClick={shoot}>Take the Shoot</button>
      {studentList}
    </div>
  );
}

export default App;
