open Utils;

let component = ReasonReact.statelessComponent("WithLoad");

let make = (~onClick, ~isLoading, children) => {
  ...component,
  render: (_) => {
    Js.log("withLoade");
    Js.log(isLoading);
    (isLoading) ? <div> (s2e("Loading...")) </div> : <Button onClick> children[0] </Button>
  }
  /* <Button
       onClick
     />
       (s2e("More"))
     </Button> */
};