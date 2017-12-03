open Utils;

let component = ReasonReact.statelessComponent("Table");

let mk_sty = (x) => ReactDOMRe.Style.make(~width=x, ());

let make = (~list, ~onDissmiss, _children) => {
  ...component,
  render: (_) =>
    <div className="table">
      <div className="table-header">
        <span style=(mk_sty("40%"))> (s2e("Title")) </span>
        <span style=(mk_sty("30%"))> (s2e("Author")) </span>
        <span style=(mk_sty("10%"))> (s2e("Comments")) </span>
        <span style=(mk_sty("10%"))> (s2e("Points")) </span>
        <span style=(mk_sty("10%"))> (s2e("Archive")) </span>
      </div>
      (
        l2e(
          list
          |> List.map(
               (item) =>
                 <div key=item.objectID className="table-row">
                   <span style=(mk_sty("40%"))> <a href=item.url> (s2e(item.title)) </a> </span>
                   <span style=(mk_sty("30%"))> (s2e(item.author)) </span>
                   <span style=(mk_sty("10%"))> (i2e(item.num_comments)) </span>
                   <span style=(mk_sty("10%"))> (i2e(item.points)) </span>
                   <span style=(mk_sty("10%"))>
                     <Button onClick=(onDissmiss(item.objectID))> (s2e("Dissmiss")) </Button>
                   </span>
                 </div>
             )
        )
      )
    </div>
};