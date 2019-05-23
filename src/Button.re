// let component = React.statelessComponent("Button");
[@react.component]
let make = (~onClick, ~className="", ~children) =>
  <button onClick className type_="button"> children </button>;