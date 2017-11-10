let component = ReasonReact.statelessComponent("Episode");

let handleClick = (_event, _self) => Js.log("clicked!");

let make = (~title, _children) => {
  ...component,
  render: (self) => <div> (ReasonReact.stringToElement(title)) </div>
};